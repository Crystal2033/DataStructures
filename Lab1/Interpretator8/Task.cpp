#include<iostream>
#include <fstream>
#include <stdlib.h>
#include<cctype>
#include <locale>
#include"Colors.h"
#include<string>
#include<regex>
#include<string>
#include<map>

/// <summary>
/// Писать раздельно left= (то есть left =) - нельзя, иначе теряется смысл контекста, не понятно, это настрока или это синоним.
/// SavedSettings должен содержать какие-либо синонимы.
/// </summary>

class MyException : public std::exception
{
private:
	//const char* err_msg; \\problem with locale h
	std::string error_msg_str;

public:
	MyException(const std::string& err)
	{
		//err_msg = err.c_str();
		error_msg_str = err;
	}
	/*const char* what() const override
	{
		std::cout << err_msg << std::endl;
		return err_msg;
	}*/
	const std::string& what_str() const
	{
		return error_msg_str;
	}

};
int add(const int first, const int second);
int mult(const int first, const int second);
int sub(const int first, const int second);
int power(const int first, const int second);
int divis(const int first, const int second);
int rem(const int first, const int second);
int my_xor(const int first, const int second);


bool is_numeric_system_correct(const std::string num, const int base)
{
	if (base > 36 || base < 2)
	{
		return false;
	}
	const std::string full_symbols_of_all_sys = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int tmp_num_sys = 0;
	int min_num_sys = 0;
	for (int i = 0; i < num.size(); i++)
	{
		tmp_num_sys = full_symbols_of_all_sys.find(toupper(num[i])) + 1;
		if (tmp_num_sys == std::string::npos)
		{
			return false;
		}
		if (tmp_num_sys > min_num_sys)
		{
			min_num_sys = tmp_num_sys;
		}
	}
	if (base < min_num_sys)
	{
		return false;
	}
	else return true;
}

void get_synonims(std::string& synonims_line, std::string* synonims)
{
	const std::string delim = ";";
	unsigned int pos = 0;
	std::string token;
	int i = 0;
	while ((pos = synonims_line.find(delim)) != std::string::npos) {
		token = synonims_line.substr(0, pos);
		synonims[i] = token;
		i++;
		synonims_line.erase(0, pos + delim.length());
	}
	token = synonims_line.substr(0, pos);
	if (token.size() != 0)
	{
		synonims[i] = token;
	}
}
int from_base_to_decimal(const std::string& num, const int base)
{
	if (!is_numeric_system_correct(num, base))
	{
		throw MyException("There is error in your numeric system and number.");
	}
	if (num[0] == '-')
	{
		if (num.length() == 1) //only -
		{
			throw MyException("There is error in your number. Your number can`t consists only '-'.");
		}
		if (!isdigit(num[1]))
		{
			throw MyException("There is error in your number. Your number can consists only one '-'.");
		}
		std::string positive_num = num;
		positive_num.erase(0, 1);
		return -from_base_to_decimal(positive_num, base);
	}
	unsigned int result = 0;
	for (int i = 0; i < num.length(); i++)
	{
		if (!isalnum(num[i]))
		{
			throw MyException("There is error in your number. It should consists only letters or digits.");
		}
		result = result * base + (isdigit(num[i]) ? num[i] - '0' : toupper(num[i]) - 'A' + 10);
	}
	return result;
}

std::string from_decimal_to_base(int value, int base)
{
	if (value < 0)
	{
		return "-" + from_decimal_to_base(-value, base);
	}
	else if (value == 0)
	{
		return "0";
	}
	if (base > 36 || base < 2)
	{
		throw MyException("There is error in your numeric system and number. It can`t be more than 36 or less than 2.");
	}

	std::string result = "";
	int remained = 0;

	while (value > 0)
	{
		remained = value % base;
		value /= base;
		result.insert(result.begin(), (remained >= 10 ? remained - 10 + 'A' : remained + '0'));
	}
	return result;
}


class Interpretator
{
private:
	std::map<const std::string, int> data;

	bool is_res_left;
	bool is_res_right;
	bool is_op_before;
	bool is_op_after;
	bool is_op_between;

	std::string assignment_symbol;

	static const int value_of_oper_list = 10;
	std::pair<std::pair<std::string, std::string>, int (*)(const int, const int)> operations_list[value_of_oper_list]; //value_of_stdrt_oper_name_bank

	void init_operations();
	void set_flags(const std::string& str);
	void parse_and_solve(const std::string& str);
	int get_num_from_var(const std::string& operand);
	int get_base_from_str(const std::string& base_str) const;
	bool is_var_not_conflict_with_oper(const std::string& var);


	void left_and_op_before(const std::string& str);
	void left_and_op_after(const std::string& str);
	void left_and_op_between_unar_bef(const std::string& str);
	void left_and_op_between_unar_aft(const std::string& str);

	void right_and_op_before(const std::string& str);
	void right_and_op_after(const std::string& str);
	void right_and_op_between_unar_bef(const std::string& str);
	void right_and_op_between_unar_aft(const std::string& str);


public:
	void set_settings(const char* settings_file_name);
	void interpretate_instr(const char* instr_file_name); //mb void is better.
	Interpretator();

};

void input_str_message(const std::string& var, const int base)
{
	std::cout << yellow << "Input in " << "<" << blue << var << yellow << ">(" << blue << base << yellow << "): " << white;
}
void output_data_message(const std::string& first, const int base, const std::string& result)
{
	std::cout << yellow << first << "(" << blue << base << yellow << ")" << white << " = " << blue << result << white << std::endl;
}

bool Interpretator::is_var_not_conflict_with_oper(const std::string& var)
{
	for (int i = 0; i < value_of_oper_list; i++)
	{
		if (var == operations_list[i].first.first)
		{
			return false;
		}
	}
	return true;
}

Interpretator::Interpretator()
{
	is_res_left = true;
	is_res_right = false;
	is_op_before = true;
	is_op_after = false;
	is_op_between = false;
	init_operations();
}
void Interpretator::set_flags(const std::string& str)
{
	if (str == "left=")
	{
		this->is_res_left = true;
		this->is_res_right = false;
		return;
	}
	else if (str == "right=")
	{
		this->is_res_left = false;
		this->is_res_right = true;
		return;
	}

	if (str == "op()")
	{
		this->is_op_before = true;
		this->is_op_after = false;
		this->is_op_between = false;
		return;
	}
	else if (str == "()op")
	{
		this->is_op_before = false;
		this->is_op_after = true;
		this->is_op_between = false;
		return;
	}
	else if (str == "(op)()")
	{
		this->is_op_before = true; //for single operations (input, output)    input(a)
		this->is_op_after = false;
		this->is_op_between = true;
		return;
	}
	else if (str == "()(op)")
	{
		this->is_op_before = false;
		this->is_op_after = true;  //for single operations (input, output) (a)input
		this->is_op_between = true;
		return;
	}
}

void Interpretator::set_settings(const char* settings_file_name = nullptr)
{
	char symbol;
	const std::string settings_bank[] = { "left=", "right=", "op()", "()op", "(op)()", "()(op)" };
	const int value_of_settings_bank = 6;
	const int value_of_stdrt_oper_name_bank = 10;
	std::string standart_operations_name_bank[] = { "add", "mult", "sub", "pow", "div", "rem", "xor", "input", "output", "=" };

	std::ifstream file_std_settings;

	//TAKING SAVED SYNONIMS
	std::string synonims_line;
	std::string buffer_str;
	std::string synonims[value_of_stdrt_oper_name_bank];
	file_std_settings.open("SavedSettings.txt");
	if (!file_std_settings.is_open())
	{
		throw MyException("File SavedSettings.txt openning error.");
	}
	//int j = 0;
	std::getline(file_std_settings, synonims_line, '\n');
	if (synonims_line.size() == 0)
	{
		file_std_settings.close();
		throw MyException("Bad SavedSettings.txt file, there are no synonims.");
	}

	get_synonims(synonims_line, synonims);
	synonims_line.clear();
	file_std_settings.close();

	for (int i = 0; i < value_of_stdrt_oper_name_bank; i++)
	{
		standart_operations_name_bank[i] = synonims[i];
		this->operations_list[i].first.first = synonims[i];
	}
	//TAKING SAVED SYNONIMS

	std::string instruction;
	if (settings_file_name != nullptr)
	{
		std::ifstream file_read;
		file_read.open(settings_file_name);
		if (!file_read.is_open())
		{
			throw MyException("File openning error.");
		}
		while (!file_read.eof())
		{

			buffer_str.clear();
			while (!file_read.eof() && (symbol = file_read.get()) != '\n')
			{
				if (symbol != EOF)
				{
					buffer_str += symbol;
				}
			}

			for (int j = 0; j < buffer_str.size(); j++)//Instructions
			{
				if (buffer_str[j] == '#')
				{
					buffer_str.erase(j);
					break;
				}
			}

			if (buffer_str.size() == 0)
			{
				continue;
			}

			instruction = buffer_str; //semantic

			bool is_instr_found = false;
			std::string what_was_found;
			for (int i = 0; i < value_of_settings_bank; i++)
			{

				if (instruction.find(settings_bank[i]) != std::string::npos) //instruction has found in settings bank
				{

					is_instr_found = true;
					what_was_found = settings_bank[i];

					for (int i = 0; i < instruction.size(); i++)
					{
						if (isspace(instruction[i]))
						{
							instruction.erase(i, 1);
							i--;
						}
					}
					if (instruction.size() != what_was_found.size())
					{
						file_read.close();
						if (file_read.bad())
						{
							throw MyException("File closing error.");
						}
						throw MyException("Setting instruction has not found. Check your setting file and try again.");
					}

					if (instruction != settings_bank[i])
					{
						continue;
					}

					set_flags(what_was_found);
					break;
				}
			}


			if (!is_instr_found) //that mean synonims or something bad.
			{
				for (int k = 0; k < instruction.size(); k++) //deleting start space symbols
				{
					if (isspace(instruction[k]))
					{
						instruction.erase(k, 1);
						k--;
					}
					else
					{
						break;
					}
				}

				for (int i = 0; i < value_of_stdrt_oper_name_bank; i++)
				{
					int find_index = -1;
					find_index = instruction.find(standart_operations_name_bank[i]);
					if (find_index != std::string::npos) //instruction has found in stdrt_opers_name bank
					{
						if (find_index != 0)
						{
							continue;
						}
						is_instr_found = true;
						what_was_found = standart_operations_name_bank[i];
						std::regex regular("([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]+)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]{1,32})");
						std::cmatch result_of_reg;
						if (!std::regex_match(instruction.c_str(), result_of_reg, regular))
						{
							file_read.close();
							if (file_read.bad())
							{
								throw MyException("File closing error.");
							}
							throw MyException("Syntax error in setting file (get synonim error).");
						}
						std::string first_word = result_of_reg[1].str();
						std::string second_word = result_of_reg[3].str();
						if (first_word != standart_operations_name_bank[i])
						{
							continue;
						}
						if (first_word.size() != what_was_found.size())
						{
							file_read.close();
							if (file_read.bad())
							{
								throw MyException("File closing error.");
							}
							throw MyException("Syntax error in setting file (get synonim error).");
						}

						for (int j = 0; j < value_of_stdrt_oper_name_bank; j++)
						{
							if (second_word == standart_operations_name_bank[j])
							{
								file_read.close();
								if (file_read.bad())
								{
									throw MyException("File closing error.");
								}
								throw MyException("There are two simillar synonims. Please, choose synonims reasonable.");
							}
						}
						standart_operations_name_bank[i] = second_word; //synonim
						operations_list[i].first.first = second_word;
						break;
					}
				}
			}

			if (!is_instr_found) //instruction is not found
			{
				file_read.close();
				if (file_read.bad())
				{
					throw MyException("File closing error.");
				}
				throw MyException("Setting instruction has not found. Check your setting file and try again.");
			}

			instruction.clear();
			//break;
		}



		file_read.close();
		if (file_read.bad())
		{
			throw MyException("File closing error.");
		}

		std::ofstream file_out;
		file_out.open("SavedSettings.txt");

		if (!file_out.is_open())
		{
			throw MyException("File openning error.");
		}
		for (int i = 0; i < value_of_stdrt_oper_name_bank; i++)
		{
			file_out << standart_operations_name_bank[i] << ';';
		}
		file_out << std::endl;
		if (this->is_res_left)
		{
			file_out << "left=" << std::endl;
		}
		else if (this->is_res_right)
		{
			file_out << "right=" << std::endl;
		}

		if (this->is_op_after && this->is_op_between)
		{
			file_out << "()(op)";
		}
		else if (this->is_op_before && this->is_op_between)
		{
			file_out << "(op)()";
		}
		else if (this->is_op_before)
		{
			file_out << "op()";
		}
		else if (this->is_op_after)
		{
			file_out << "()op";
		}

		file_out.close();
		if (file_out.bad())
		{
			throw MyException("File closing error.");
		}
	}
	else
	{
		std::cout << green << "No settings. Using old file with settings." << white << std::endl;
		std::ifstream file_std_settings;
		file_std_settings.open("SavedSettings.txt");
		if (!file_std_settings.is_open())
		{
			throw MyException("File openning for reading saved settings error.");
		}


		while ((symbol = file_std_settings.get()) != '\n' && (symbol != EOF)) //taking synonims
		{
			synonims_line += symbol;
		}
		get_synonims(synonims_line, synonims); //this string must be in project!!!
		for (int i = 0; i < value_of_stdrt_oper_name_bank; i++)
		{
			standart_operations_name_bank[i] = synonims[i];
			operations_list[i].first.first = synonims[i];
		}
		synonims_line.clear();
		std::string settings_line;
		while (!file_std_settings.eof())
		{
			std::getline(file_std_settings, settings_line);
			set_flags(settings_line);
		}

		file_std_settings.close();
		if (file_std_settings.bad())
		{
			throw MyException("File closing error.");
		}
	}
}

void Interpretator::interpretate_instr(const char* instr_file_name)
{
	assignment_symbol = operations_list[9].first.first;
	std::ifstream file_with_instr;
	std::string instruction;
	file_with_instr.open(instr_file_name);
	if (!file_with_instr.is_open())
	{
		throw MyException("Can`t open file with instructions.");
	}
	char symbol;
	int in_comment = 0;
	int line_counter = 0;

	while (!file_with_instr.eof())
	{
		symbol = (char)file_with_instr.get();
		if (!this->is_op_between) //с этой операцией все сложнее (с точки зрения парсинга: a = remainininin_my_tmp)
		{
			if (isspace(symbol))
			{
				continue;
			}
		}
		if (symbol == '\n')
		{
			continue;
		}

		if (symbol == ']' && !in_comment)
		{
			file_with_instr.close();
			if (file_with_instr.bad())
			{
				throw MyException("File closing error.");
			}
			throw MyException("Was found closed bracket without open bracket.");
		}
		else if (symbol == '[' || in_comment)
		{
			if (symbol == '[')
			{
				in_comment++;
			}

			while (!file_with_instr.eof() && symbol != ']')
			{
				symbol = (char)file_with_instr.get();

				if (symbol == '[')
				{
					in_comment++;
				}
			}

			if (file_with_instr.eof() && in_comment)
			{
				file_with_instr.close();
				if (file_with_instr.bad())
				{
					throw MyException("File closing error.");
				}
				throw MyException("There is no closed bracket for opened bracket.");
			}

			if (symbol == ']')
			{
				in_comment--;
			}
			continue;
		}
		else if (symbol == ']')
		{
			in_comment--;
			continue;
		}

		if (!in_comment && symbol == ';')// (probably) GOOD INSTRUCTION. PARSING INSTRUCTION.
		{
			//std::cout << azure << instruction << white << std::endl;
			line_counter++;
			std::cout << blue << "-----------------------------------------" << white << std::endl;
			std::cout << cyan << "Line №" << line_counter << ". " << white << std::endl;
			std::cout << "Written instruction: " << cyan << instruction << white << std::endl << std::endl;
			parse_and_solve(instruction);
			std::cout << "Interpretate status: " << green << "Success!" << white << std::endl;
			//std::cout << yellow << "-----------------------------------------" << white << std::endl;
			instruction.clear();
			continue;
		}

		if (symbol == EOF && instruction.length() != 0)
		{
			if (this->is_op_between) //так как мы сохраняем проблемы в записи, где op посередине, то обрабатывать конец файла нужно так:
			{
				int space_counter = 0;
				for (int j = 0; j < instruction.length(); j++)
				{
					if (isspace(instruction[j]))
					{
						space_counter++;
					}
				}
				if (space_counter == instruction.length())
				{
					file_with_instr.close();
					if (file_with_instr.bad())
					{
						throw MyException("File closing error.");
					}
					return;
				}
			}

			if (file_with_instr.bad())
			{
				throw MyException("File closing error.");
			}
			throw MyException("Your instruction is incorrect.");
		}

		instruction += symbol;

	}
	if (line_counter)
	{
		std::cout << blue << "-----------------------------------------" << white << std::endl;
	}
	file_with_instr.close();
	if (file_with_instr.bad())
	{
		throw MyException("File closing error.");
	}
	return;
}


int Interpretator::get_num_from_var(const std::string& operand)
{
	int operand_number;
	if (isdigit(operand[0]) || operand[0] == '-') //либо это число; либо это переменная, начинающаяся с цифры.
	{
		if (!is_numeric_system_correct(operand, 10))
		{
			throw MyException("Variable cannot start with a digit or punctuation symbol.");
		}
		operand_number = from_base_to_decimal(operand, 10);
	}
	else
	{
		auto it = data.find(operand);
		if (it == data.end())
		{
			std::pair<const std::string, int> tmp_pair(operand, 0);
			data.insert(tmp_pair);
			return 0;
			//throw MyException("Variable not found.");
		}
		operand_number = (*it).second;
	}
	return operand_number;
}


int Interpretator::get_base_from_str(const std::string& base_str) const
{
	int base;
	if (isdigit(base_str[0])) //либо это число; либо это переменная, начинающаяся с цифры.
	{
		if (!is_numeric_system_correct(base_str, 10))
		{
			throw MyException("Variable cannot start with a digit.");
		}
		base = from_base_to_decimal(base_str, 10);

	}
	else
	{
		auto it = data.find(base_str);
		if (it == data.end())
		{
			throw MyException("Variable not found.");
		}
		base = (*it).second;
	}
	if (base > 36 || base < 2)
	{
		throw MyException("There is error in your numeric system and number. It can`t be more than 36 or less than 2.");
	}
	return base;
}

void Interpretator::left_and_op_before(const std::string& str)
{
	std::string first_operand_str;
	std::string second_operand_str;
	int first_operand_number;
	int second_operand_number;
	std::pair<std::string, int> pair_data;
	std::string base_str;
	int base;
	std::string result_of_instr_str;
	int total_result_num;
	int index_of_assign;
	std::string input_str;
	std::regex regular;
	std::cmatch result_of_reg;
	std::string tmp_str;
	bool found_operation = false;
	int index_of_find = -1;

	for (int i = 0; i < this->value_of_oper_list - 1; i++) //в последнем элементе лежит знак операции присваивания, ее не нужно искать.
	{
		index_of_find = str.find(this->operations_list[i].first.first);
		if (index_of_find != std::string::npos) //найдена какая-то инструкция.
		{
			first_operand_str.clear();
			second_operand_str.clear();
			result_of_instr_str.clear();
			base_str.clear();
			tmp_str.clear();
			if (index_of_find == 0) //output. OR ERROR.
			{
				regular = "([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([\\(])([\\w\\-]+)([\\,]*)([\\w]*)([\\)])";
				if (!std::regex_match(str.c_str(), result_of_reg, regular))
				{
					throw MyException("Wrong instruction. Regular check error.");
				}
				if (result_of_reg[1] != this->operations_list[i].first.first || (this->operations_list[i].first.second != "<<"))
				{
					continue;
				}

				found_operation = true;
				first_operand_str = result_of_reg[3];
				first_operand_number = get_num_from_var(first_operand_str);
				std::string help_str;
				help_str = result_of_reg[4];
				if (result_of_reg[5].length() != 0)
				{
					if (help_str.length() > 1)
					{
						throw MyException("Too many <,> in your instruction.");
					}
					base_str = result_of_reg[5];
					base = get_base_from_str(base_str);
				}
				else
				{
					if (help_str.length() > 0)
					{
						throw MyException("Your instruction doesn`t have to consist <,>.");
					}
					base = 10;
				}

				result_of_instr_str = from_decimal_to_base(first_operand_number, base);
				output_data_message(first_operand_str, base, result_of_instr_str);
				//std::cout << yellow << first_operand_str << "(" << blue << base << yellow << ")" << white << " = " << blue << result_of_instr_str << white <<std::endl;
				break;

			}
			else
			{
				index_of_assign = str.find(assignment_symbol);
				if (index_of_assign == std::string::npos)
				{
					continue;
				}
				for (int j = 0; j < index_of_assign; j++)
				{
					if (!isalnum(str[j]))
					{
						throw MyException("Variable can`t consists punctuation symbols.");
					}
					result_of_instr_str += str[j];
				}
				if (isdigit(result_of_instr_str[0]))
				{
					throw MyException("Variable can`t starts with a digit.");
				}

				if (!is_var_not_conflict_with_oper(result_of_instr_str))
				{
					throw MyException("Conflict between operation and variable names.");
				}
				tmp_str = str;
				tmp_str.erase(0, index_of_assign + assignment_symbol.length());
				regular = "([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)(\\()([\\w\\-]+)(\\,)([\\w\\-]+)(\\))";
				if (!std::regex_match(tmp_str.c_str(), result_of_reg, regular)) //could be input
				{
					if (this->operations_list[i].first.second == ">>") //input
					{
						regular = "([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)(\\()([\\w]*)(\\))";
						if (!std::regex_match(tmp_str.c_str(), result_of_reg, regular))
						{
							throw MyException("Wrong instruction. Regular check error.");
						}
						if (result_of_reg[1] != this->operations_list[i].first.first) // IN PRINT
						{
							continue;
						}
						if (result_of_reg[3].length() != 0)
						{
							base_str = result_of_reg[3];
							base = get_base_from_str(base_str);
						}
						else
						{
							base = 10;
						}
						found_operation = true;
						input_str_message(result_of_instr_str, base);
						//std::cout << yellow << "Input in " << "<" <<cyan << result_of_instr_str <<yellow <<">(" << blue << base <<yellow << "): " << white;
						std::cin >> input_str;
						total_result_num = from_base_to_decimal(input_str, base);
						pair_data.first = result_of_instr_str;
						pair_data.second = total_result_num;
						auto it = data.find(pair_data.first);
						if (it != data.end())
						{
							data[result_of_instr_str] = total_result_num;
							break;
						}
						data.insert(pair_data);
						break;

					}

				}
				if (result_of_reg[1] != this->operations_list[i].first.first) // addd
				{
					continue;
				}

				found_operation = true;
				first_operand_str = result_of_reg[3];
				first_operand_number = get_num_from_var(first_operand_str);
				second_operand_str = result_of_reg[5];
				second_operand_number = get_num_from_var(second_operand_str);
				total_result_num = this->operations_list[i].second(first_operand_number, second_operand_number);
				pair_data.first = result_of_instr_str;
				pair_data.second = total_result_num;

				auto it = data.find(pair_data.first);
				if (it != data.end())
				{
					data[result_of_instr_str] = total_result_num;
					break;
				}
				data.insert(pair_data);
				break;
			}
		}
	}
	if (!found_operation)
	{
		throw MyException("Operation not found.");
	}
}

void Interpretator::left_and_op_after(const std::string& str)
{
	std::string first_operand_str;
	std::string second_operand_str;
	int first_operand_number;
	int second_operand_number;
	std::pair<std::string, int> pair_data;
	std::string base_str;
	int base;
	std::string result_of_instr_str;
	int total_result_num;
	int index_of_assign;
	std::string input_str;
	std::regex regular;
	std::cmatch result_of_reg;
	std::string tmp_str;
	bool found_operation = false;
	int index_of_find = -1;
	for (int i = 0; i < this->value_of_oper_list - 1; i++) //в последнем элементе лежит знак операции присваивания, ее не нужно искать.
	{
		index_of_find = str.find(this->operations_list[i].first.first);
		if (index_of_find != std::string::npos) //найдена какая-то инструкция.
		{
			first_operand_str.clear();
			second_operand_str.clear();
			result_of_instr_str.clear();
			base_str.clear();
			tmp_str.clear();
			if (str[0] == '(') //output. OR ERROR.
			{
				regular = "([\\(])([\\w\\-]+)([\\,]*)([\\w]*)([\\)])([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)";
				if (!std::regex_match(str.c_str(), result_of_reg, regular))
				{
					throw MyException("Wrong instruction. Regular check error.");
				}
				std::string help_str;
				help_str = result_of_reg[3];
				if (help_str.length() > 1)
				{
					throw MyException("Too many <,> in your instruction.");
				}

				if (result_of_reg[6] != this->operations_list[i].first.first || (this->operations_list[i].first.second != "<<"))//in print
				{
					continue;
				}

				found_operation = true;
				first_operand_str = result_of_reg[2];
				first_operand_number = get_num_from_var(first_operand_str);
				//std::string help_str;
				help_str = result_of_reg[3];
				if (result_of_reg[4].length() != 0)
				{
					base_str = result_of_reg[4];
					base = get_base_from_str(base_str);
				}
				else
				{
					if (help_str.length() > 0)
					{
						throw MyException("Your instruction doesn`t have to consist <,>.");
					}

					base = 10;

				}
				result_of_instr_str = from_decimal_to_base(first_operand_number, base);
				output_data_message(first_operand_str, base, result_of_instr_str);
				//std::cout << yellow << first_operand_str << "(" << blue << base << yellow << ")" << white << " = " << blue << result_of_instr_str << white << std::endl;
				break;
			}
			else
			{
				index_of_assign = str.find(assignment_symbol);
				if (index_of_assign == std::string::npos)
				{
					continue;
				}
				for (int j = 0; j < index_of_assign; j++)
				{
					if (!isalnum(str[j]))
					{
						throw MyException("Variable can`t consists punctuation symbols.");
					}
					result_of_instr_str += str[j];
				}
				if (isdigit(result_of_instr_str[0]))
				{
					throw MyException("Variable can`t starts with a digit.");
				}

				if (!is_var_not_conflict_with_oper(result_of_instr_str))
				{
					throw MyException("Conflict between operation and variable names.");
				}
				tmp_str = str;
				tmp_str.erase(0, index_of_assign + assignment_symbol.length());
				regular = "(\\()([\\w\\-]+)(\\,)([\\w\\-]+)(\\))([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)";
				if (!std::regex_match(tmp_str.c_str(), result_of_reg, regular)) //could be input
				{
					if (this->operations_list[i].first.second == ">>") //input
					{
						regular = "(\\()([\\w]*)(\\))([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)";
						if (!std::regex_match(tmp_str.c_str(), result_of_reg, regular))
						{
							throw MyException("Wrong instruction. Regular check error.");
						}

						if (result_of_reg[2].length() == 0)
						{
							if (result_of_reg[4] != this->operations_list[i].first.first) // in and print
							{
								continue;
							}
						}
						else
						{
							if (result_of_reg[4] != this->operations_list[i].first.first) // 
							{
								continue;
							}
						}

						if (result_of_reg[2].length() != 0)
						{
							base_str = result_of_reg[2];
							base = get_base_from_str(base_str);
						}
						else
						{
							base = 10;
						}
						found_operation = true;
						input_str_message(result_of_instr_str, base);
						//std::cout << yellow << "Input in " << "<" << cyan << result_of_instr_str << yellow << ">(" << blue << base << yellow << "): " << white;
						std::cin >> input_str;
						total_result_num = from_base_to_decimal(input_str, base);
						pair_data.first = result_of_instr_str;
						pair_data.second = total_result_num;
						auto it = data.find(pair_data.first);
						if (it != data.end())
						{
							data[result_of_instr_str] = total_result_num;
							break;
						}
						data.insert(pair_data);
						break;

					}
				}
				if (result_of_reg[6] != this->operations_list[i].first.first) // addd
				{
					continue;
				}

				found_operation = true;
				first_operand_str = result_of_reg[2];
				first_operand_number = get_num_from_var(first_operand_str);
				second_operand_str = result_of_reg[4];
				second_operand_number = get_num_from_var(second_operand_str);
				total_result_num = this->operations_list[i].second(first_operand_number, second_operand_number);
				pair_data.first = result_of_instr_str;
				pair_data.second = total_result_num;

				auto it = data.find(pair_data.first);
				if (it != data.end())
				{
					data[result_of_instr_str] = total_result_num;
					break;
				}
				data.insert(pair_data);
				break;
			}
		}
	}
	if (!found_operation)
	{
		throw MyException("Operation not found.");
	}
}

void Interpretator::left_and_op_between_unar_bef(const std::string& str)
{
	std::string first_operand_str;
	std::string second_operand_str;
	int first_operand_number;
	int second_operand_number;
	std::pair<std::string, int> pair_data;
	std::string base_str;
	int base;
	std::string result_of_instr_str;
	int total_result_num;
	int index_of_assign;
	std::string input_str;
	std::regex regular;
	std::cmatch result_of_reg;
	std::string tmp_str;
	bool found_operation = false;
	int index_of_find = -1;
	for (int i = 0; i < this->value_of_oper_list - 1; i++)
	{
		index_of_find = str.find(this->operations_list[i].first.first);
		if (index_of_find != std::string::npos) //что-то нашлось
		{

			first_operand_str.clear();
			second_operand_str.clear();
			result_of_instr_str.clear();
			base_str.clear();
			tmp_str.clear();

			index_of_assign = str.find(assignment_symbol);
			if (index_of_assign == std::string::npos) //output или error
			{

				if (this->operations_list[i].first.second != "<<")  //(addbar)output;
				{
					continue;
				}

				regular = "([ ]*)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]*)([\\(])([ ]*)([\\w\\-]+)([ ]*)([\\)])([ ]*)";
				if (std::regex_match(str.c_str(), result_of_reg, regular)) // output(what_to_output);
				{
					if (result_of_reg[2] != this->operations_list[i].first.first)
					{
						throw MyException("Wrong output instruction. Or you have forgotten an assignment simbol.");
					}

					found_operation = true;
					first_operand_str = result_of_reg[6];
					first_operand_number = get_num_from_var(first_operand_str);
					base = 10;

					result_of_instr_str = from_decimal_to_base(first_operand_number, base);
					output_data_message(first_operand_str, base, result_of_instr_str);
					break;

				}
				//else
				//a output b;
				regular = "([ ]*)([\\w\\-]+)([ ]+)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]+)([\\w\\-]+)([ ]*)";
				if (std::regex_match(str.c_str(), result_of_reg, regular))
				{
					found_operation = true;
					first_operand_str = result_of_reg[2];
					first_operand_number = get_num_from_var(first_operand_str);

					base_str = result_of_reg[6];
					base = get_base_from_str(base_str);

					result_of_instr_str = from_decimal_to_base(first_operand_number, base);
					output_data_message(first_operand_str, base, result_of_instr_str);
				}
				else
				{
					throw MyException("Wrong output operation syntaxis. Please, try again. May be you have forgotten space symbols.");
				}
			}
			else //значит, что-то нашлось с знаком равно.
			{
				for (int j = 0; j < index_of_assign; j++)
				{
					if (!isspace(str[j]))
					{
						if (!isalnum(str[j]))
						{
							throw MyException("Variable can`t consists punctuation symbols.");
						}
						result_of_instr_str += str[j];
					}
				}
				if (isdigit(result_of_instr_str[0]))
				{
					throw MyException("Variable can`t starts with a digit.");
				}
				if (!is_var_not_conflict_with_oper(result_of_instr_str))
				{
					throw MyException("Conflict between operation and variable names.");
				}
				tmp_str = str;
				tmp_str.erase(0, index_of_assign + assignment_symbol.length());

				regular = "([ ]*)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]*)([\\(])([ ]*)([\\w]*)([ ]*)([\\)])([ ]*)"; //Отловим шаблон input
				if (std::regex_match(tmp_str.c_str(), result_of_reg, regular))
				{
					//если нашлось, то это наш input или ошибка.
					if (this->operations_list[i].first.second != ">>") //addbar= input(2);
					{
						continue;
						//throw MyException("Wrong input operation.");
					}
					if (result_of_reg[2] != this->operations_list[i].first.first)
					{
						continue;
						//throw MyException("Wrong instruction. Regular check error.");
					}


					if (result_of_reg[6].length() != 0) //input(base)=a
					{
						base_str = result_of_reg[6];
						base = get_base_from_str(base_str);

					}
					else //input() = a
					{
						base = 10;
					}

					found_operation = true;
					input_str_message(result_of_instr_str, base);
					std::cin >> input_str;
					total_result_num = from_base_to_decimal(input_str, base);
					pair_data.first = result_of_instr_str;
					pair_data.second = total_result_num;
					auto it = data.find(pair_data.first);
					if (it != data.end())
					{
						data[result_of_instr_str] = total_result_num;
						break;
					}

					data.insert(pair_data);
					break;
				}
				//остальные операции (add, sub, mult и т.д.)
				regular = "([ ]*)([\\w\\-]+)([ ]+)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]+)([\\w\\-]+)([ ]*)";
				if (!std::regex_match(tmp_str.c_str(), result_of_reg, regular)) //нашлась инструкция
				{
					throw MyException("Error syntaxis of your instruction, please, check it out and try again.");
				}

				if (result_of_reg[4] != this->operations_list[i].first.first) // addd
				{
					continue;
				}

				found_operation = true;
				first_operand_str = result_of_reg[2];
				first_operand_number = get_num_from_var(first_operand_str);
				second_operand_str = result_of_reg[6];
				second_operand_number = get_num_from_var(second_operand_str);
				total_result_num = this->operations_list[i].second(first_operand_number, second_operand_number);
				pair_data.first = result_of_instr_str;
				pair_data.second = total_result_num;

				auto it = data.find(pair_data.first);
				if (it != data.end())
				{
					data[result_of_instr_str] = total_result_num;
					break;
				}
				data.insert(pair_data);
				break;
			}
		}
	}
	if (!found_operation)
	{
		throw MyException("Operation not found.");
	}
}

void Interpretator::left_and_op_between_unar_aft(const std::string& str)
{
	std::string first_operand_str;
	std::string second_operand_str;
	int first_operand_number;
	int second_operand_number;
	std::pair<std::string, int> pair_data;
	std::string base_str;
	int base;
	std::string result_of_instr_str;
	int total_result_num;
	int index_of_assign;
	std::string input_str;
	std::regex regular;
	std::cmatch result_of_reg;
	std::string tmp_str;
	bool found_operation = false;
	int index_of_find = -1;

	for (int i = 0; i < this->value_of_oper_list - 1; i++)
	{
		index_of_find = str.find(this->operations_list[i].first.first);
		if (index_of_find != std::string::npos) //что-то нашлось
		{

			first_operand_str.clear();
			second_operand_str.clear();
			result_of_instr_str.clear();
			base_str.clear();
			tmp_str.clear();

			index_of_assign = str.find(assignment_symbol);
			if (index_of_assign == std::string::npos) //output или error
			{

				if (this->operations_list[i].first.second != "<<")  //(addbar)output;
				{
					continue;
				}

				regular = "([ ]*)([\\(])([ ]*)([\\w\\-]+)([ ]*)([\\)])([ ]*)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]*)";
				if (std::regex_match(str.c_str(), result_of_reg, regular)) // output(what_to_output);
				{
					if (result_of_reg[8] != this->operations_list[i].first.first)
					{
						throw MyException("Wrong output instruction. Or you have forgotten an assignment simbol.");
					}

					found_operation = true;
					first_operand_str = result_of_reg[4];
					first_operand_number = get_num_from_var(first_operand_str);
					base = 10;

					result_of_instr_str = from_decimal_to_base(first_operand_number, base);
					output_data_message(first_operand_str, base, result_of_instr_str);
					break;

				}
				//else
				//a output b;
				regular = "([ ]*)([\\w\\-]+)([ ]+)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]+)([\\w\\-]+)([ ]*)";
				if (std::regex_match(str.c_str(), result_of_reg, regular))
				{
					found_operation = true;
					first_operand_str = result_of_reg[2];
					first_operand_number = get_num_from_var(first_operand_str);

					base_str = result_of_reg[6];
					base = get_base_from_str(base_str);

					result_of_instr_str = from_decimal_to_base(first_operand_number, base);
					output_data_message(first_operand_str, base, result_of_instr_str);
				}
				else
				{
					throw MyException("Wrong output operation syntaxis. Please, try again. May be you have forgotten space symbols.");
				}
			}
			else //значит, что-то нашлось с знаком равно.
			{
				for (int j = 0; j < index_of_assign; j++)
				{
					if (!isspace(str[j]))
					{
						if (!isalnum(str[j]))
						{
							throw MyException("Variable can`t consists punctuation symbols.");
						}
						result_of_instr_str += str[j];
					}
				}
				if (isdigit(result_of_instr_str[0]))
				{
					throw MyException("Variable can`t starts with a digit.");
				}
				if (!is_var_not_conflict_with_oper(result_of_instr_str))
				{
					throw MyException("Conflict between operation and variable names.");
				}
				tmp_str = str;
				tmp_str.erase(0, index_of_assign + assignment_symbol.length());

				regular = "([ ]*)([\\(])([ ]*)([\\w]*)([ ]*)([\\)])([ ]*)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]*)"; //Отловим шаблон input
				if (std::regex_match(tmp_str.c_str(), result_of_reg, regular))
				{
					//если нашлось, то это наш input или ошибка.
					if (this->operations_list[i].first.second != ">>")
					{
						continue;
					}
					if (result_of_reg[8] != this->operations_list[i].first.first)
					{
						continue;
					}


					if (result_of_reg[4].length() != 0) //(base)input=a
					{
						base_str = result_of_reg[4];
						base = get_base_from_str(base_str);

					}
					else //input() = a
					{
						base = 10;
					}

					found_operation = true;
					input_str_message(result_of_instr_str, base);
					std::cin >> input_str;
					total_result_num = from_base_to_decimal(input_str, base);
					pair_data.first = result_of_instr_str;
					pair_data.second = total_result_num;
					auto it = data.find(pair_data.first);
					if (it != data.end())
					{
						data[result_of_instr_str] = total_result_num;
						break;
					}

					data.insert(pair_data);
					break;
				}
				//остальные операции (add, sub, mult и т.д.)
				regular = "([ ]*)([\\w\\-]+)([ ]+)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]+)([\\w\\-]+)([ ]*)";
				if (!std::regex_match(tmp_str.c_str(), result_of_reg, regular)) //нашлась инструкция
				{
					throw MyException("Error syntaxis of your instruction, please, check it out and try again.");
				}

				if (result_of_reg[4] != this->operations_list[i].first.first) // addd
				{
					continue;
				}

				found_operation = true;
				first_operand_str = result_of_reg[2];
				first_operand_number = get_num_from_var(first_operand_str);
				second_operand_str = result_of_reg[6];
				second_operand_number = get_num_from_var(second_operand_str);
				total_result_num = this->operations_list[i].second(first_operand_number, second_operand_number);
				pair_data.first = result_of_instr_str;
				pair_data.second = total_result_num;

				auto it = data.find(pair_data.first);
				if (it != data.end())
				{
					data[result_of_instr_str] = total_result_num;
					break;
				}
				data.insert(pair_data);
				break;
			}
		}

	}
	if (!found_operation)
	{
		throw MyException("Operation not found.");
	}
}

void Interpretator::right_and_op_before(const std::string& str)
{
	std::string first_operand_str;
	std::string second_operand_str;
	int first_operand_number;
	int second_operand_number;
	std::pair<std::string, int> pair_data;
	std::string base_str;
	int base;
	std::string result_of_instr_str;
	int total_result_num;
	int index_of_assign;
	std::string input_str;
	std::regex regular;
	std::cmatch result_of_reg;
	std::string tmp_str;
	bool found_operation = false;
	int index_of_find = -1;

	for (int i = 0; i < this->value_of_oper_list - 1; i++) //в последнем элементе лежит знак операции присваивания, ее не нужно искать.
	{
		index_of_find = str.find(this->operations_list[i].first.first);
		if (index_of_find != std::string::npos) //найдена какая-то инструкция.
		{
			first_operand_str.clear();
			second_operand_str.clear();
			result_of_instr_str.clear();
			base_str.clear();
			tmp_str.clear();
			if (index_of_find != 0) //tmp with name: add1 or error.
			{
				continue;
			}

			index_of_assign = str.find(assignment_symbol);
			if (index_of_assign == std::string::npos) //output or Error
			{
				if (this->operations_list[i].first.second != "<<")  //output(intmp);
				{
					continue;
				}

				regular = "([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([\\(])([\\w\\-]+)([\\,]*)([\\w]*)([\\)])";
				if (!std::regex_match(str.c_str(), result_of_reg, regular))
				{
					throw MyException("Wrong instruction. Regular check error.");
				}

				if (result_of_reg[1] != this->operations_list[i].first.first)
				{
					throw MyException("Wrong instruction. Output instruction failed.");
				}


				found_operation = true;
				first_operand_str = result_of_reg[3];
				first_operand_number = get_num_from_var(first_operand_str);
				std::string help_str;
				help_str = result_of_reg[4];

				if (help_str.length() > 1)
				{
					throw MyException("Too many <,> in your instruction.");
				}

				if (result_of_reg[5].length() != 0)
				{
					base_str = result_of_reg[5];
					base = get_base_from_str(base_str);
				}
				else
				{
					if (help_str.length() > 0)
					{
						throw MyException("Your instruction doesn`t have to consist <,>.");
					}
					base = 10;
				}

				result_of_instr_str = from_decimal_to_base(first_operand_number, base);
				output_data_message(first_operand_str, base, result_of_instr_str);
				break;

			}


			if (this->operations_list[i].first.second == ">>") //input oper
			{
				for (int j = index_of_assign + assignment_symbol.length(); j < str.length(); j++)  //input(a) = asd2
				{
					if (!isalnum(str[j]))
					{
						throw MyException("Variable can`t consists punctuation symbols.");
					}
					result_of_instr_str += str[j];
				}
				if (isdigit(result_of_instr_str[0]))
				{
					throw MyException("Variable can`t starts with a digit.");
				}
				if (!is_var_not_conflict_with_oper(result_of_instr_str))
				{
					throw MyException("Conflict between operation and variable names.");
				}
				tmp_str = str;
				tmp_str.erase(index_of_assign);
				regular = "([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)(\\()([\\w]*)(\\))";
				if (!std::regex_match(tmp_str.c_str(), result_of_reg, regular))
				{
					throw MyException("Wrong instruction. Regular check error.");
				}

				if (result_of_reg[1] != this->operations_list[i].first.first) //(addbar)input
				{
					continue;
					//throw MyException("Wrong instruction. Input instruction failed.");
				}


				if (result_of_reg[3].length() != 0)
				{
					base_str = result_of_reg[3];
					base = get_base_from_str(base_str);
				}
				else
				{
					base = 10;
				}
				found_operation = true;
				input_str_message(result_of_instr_str, base);
				std::cin >> input_str;
				total_result_num = from_base_to_decimal(input_str, base);
				pair_data.first = result_of_instr_str;
				pair_data.second = total_result_num;
				auto it = data.find(pair_data.first);
				if (it != data.end())
				{
					data[result_of_instr_str] = total_result_num;
					break;
				}
				data.insert(pair_data);
				break;
			}
			else //others operations
			{

				for (int j = index_of_assign + assignment_symbol.length(); j < str.length(); j++)  //input(a) = asd2
				{
					if (!isalnum(str[j]))
					{
						throw MyException("Variable can`t consists punctuation symbols.");
					}
					result_of_instr_str += str[j];
				}
				if (isdigit(result_of_instr_str[0]))
				{
					throw MyException("Variable can`t starts with a digit.");
				}
				if (!is_var_not_conflict_with_oper(result_of_instr_str))
				{
					throw MyException("Conflict between operation and variable names.");
				}
				tmp_str = str;
				tmp_str.erase(index_of_assign);
				regular = "([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)(\\()([\\w\\-]+)(\\,)([\\w\\-]+)(\\))";
				if (!std::regex_match(tmp_str.c_str(), result_of_reg, regular))
				{
					throw MyException("Wrong instruction. Regular check error.");
				}
				if (result_of_reg[1] != this->operations_list[i].first.first) // addd
				{
					continue;
				}


				found_operation = true;
				first_operand_str = result_of_reg[3];
				first_operand_number = get_num_from_var(first_operand_str);
				second_operand_str = result_of_reg[5];
				second_operand_number = get_num_from_var(second_operand_str);
				total_result_num = this->operations_list[i].second(first_operand_number, second_operand_number);
				pair_data.first = result_of_instr_str;
				pair_data.second = total_result_num;

				auto it = data.find(pair_data.first);
				if (it != data.end())
				{
					data[result_of_instr_str] = total_result_num;
					break;
				}
				data.insert(pair_data);
				break;
			}
		}
	}
	if (!found_operation)
	{
		throw MyException("Operation not found.");
	}
}

void Interpretator::right_and_op_after(const std::string& str)
{
	std::string first_operand_str;
	std::string second_operand_str;
	int first_operand_number;
	int second_operand_number;
	std::pair<std::string, int> pair_data;
	std::string base_str;
	int base;
	std::string result_of_instr_str;
	int total_result_num;
	int index_of_assign;
	std::string input_str;
	std::regex regular;
	std::cmatch result_of_reg;
	std::string tmp_str;
	bool found_operation = false;
	int index_of_find = -1;

	for (int i = 0; i < this->value_of_oper_list - 1; i++) //в последнем элементе лежит знак операции присваивания, ее не нужно искать.
	{
		if (str[0] != '(')
		{
			throw MyException("Wrong instruction. It starts not at '('.");
		}
		index_of_find = str.find(this->operations_list[i].first.first);
		if (index_of_find != std::string::npos) //что-то нашлось
		{
			first_operand_str.clear();
			second_operand_str.clear();
			result_of_instr_str.clear();
			base_str.clear();
			tmp_str.clear();

			index_of_assign = str.find(assignment_symbol);
			if (index_of_assign == std::string::npos) //output or err.
			{
				if (this->operations_list[i].first.second != "<<")  //(intmp)output;
				{
					continue;
				}

				regular = "([\\(])([\\w\\-]+)([\\,]*)([\\w]*)([\\)])([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)";
				if (!std::regex_match(str.c_str(), result_of_reg, regular))
				{
					throw MyException("Wrong instruction. Regular check error.");
				}
				if (result_of_reg[6] != this->operations_list[i].first.first) //(16)output=intmp;
				{
					throw MyException("Wrong instruction. Output instruction error.");
				}

				found_operation = true;
				first_operand_str = result_of_reg[2];
				first_operand_number = get_num_from_var(first_operand_str);
				std::string help_str;
				help_str = result_of_reg[3];
				if (help_str.length() > 1)
				{
					throw MyException("Too many <,> in your instruction.");
				}
				if (result_of_reg[4].length() != 0)
				{
					base_str = result_of_reg[4];
					base = get_base_from_str(base_str);
				}
				else
				{
					if (help_str.length() > 0)
					{
						throw MyException("Your instruction doesn`t have to consist <,>.");
					}

					base = 10;
				}

				result_of_instr_str = from_decimal_to_base(first_operand_number, base);
				output_data_message(first_operand_str, base, result_of_instr_str);
				break;
			}
			else //(add_base)input=v
			{
				for (int j = index_of_assign + assignment_symbol.length(); j < str.length(); j++)  //(a)input = asd2
				{
					if (!isalnum(str[j]))
					{
						throw MyException("Variable can`t consists punctuation symbols.");
					}
					result_of_instr_str += str[j];
				}
				if (isdigit(result_of_instr_str[0]))
				{
					throw MyException("Variable can`t starts with a digit.");
				}
				if (!is_var_not_conflict_with_oper(result_of_instr_str))
				{
					throw MyException("Conflict between operation and variable names.");
				}
				tmp_str = str;
				tmp_str.erase(index_of_assign);
				regular = "([\\(])([\\w]*)([\\,]*)([\\w]*)([\\)])([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)";
				if (!std::regex_match(tmp_str.c_str(), result_of_reg, regular))
				{
					throw MyException("Wrong operation. Please, check your instruction better.");
				}
				std::string help_str;
				help_str = result_of_reg[3];
				if (help_str.length() > 1)
				{
					throw MyException("Too many <,> in your instruction.");
				}

				if (result_of_reg[4].length() == 0) //input or error. ()input=a or (base)input=a
				{
					if (this->operations_list[i].first.second != ">>")
					{ //(2)input=addbar
						continue;
						//throw MyException("Wrong input operation.");
					}
					if (result_of_reg[6] != this->operations_list[i].first.first)// TMP - ADD, ADD - OPERATION
					{
						continue;
						//throw MyException("Wrong instruction. Regular check error.");
					}
					if (help_str.length() > 0)
					{
						throw MyException("Your instruction doesn`t have to consist <,>.");
					}

					if (result_of_reg[2].length() != 0) //(base)input=a
					{
						base_str = result_of_reg[2];
						base = get_base_from_str(base_str);

					}
					else //()input = a
					{
						base = 10;
					}

					found_operation = true;
					input_str_message(result_of_instr_str, base);
					std::cin >> input_str;
					total_result_num = from_base_to_decimal(input_str, base);
					pair_data.first = result_of_instr_str;
					pair_data.second = total_result_num;
					auto it = data.find(pair_data.first);
					if (it != data.end())
					{
						data[result_of_instr_str] = total_result_num;
						break;
					}

					data.insert(pair_data);
					break;
				}
				else //other opearations
				{
					if (result_of_reg[6] != this->operations_list[i].first.first) // addd
					{
						continue;
					}

					found_operation = true;
					first_operand_str = result_of_reg[2];
					first_operand_number = get_num_from_var(first_operand_str);
					second_operand_str = result_of_reg[4];
					second_operand_number = get_num_from_var(second_operand_str);
					total_result_num = this->operations_list[i].second(first_operand_number, second_operand_number);
					pair_data.first = result_of_instr_str;
					pair_data.second = total_result_num;

					auto it = data.find(pair_data.first);
					if (it != data.end())
					{
						data[result_of_instr_str] = total_result_num;
						break;
					}
					data.insert(pair_data);
					break;
				}
			}
		}
	}
	if (!found_operation)
	{
		throw MyException("Operation not found.");
	}
}

void Interpretator::right_and_op_between_unar_bef(const std::string& str)
{
	std::string first_operand_str;
	std::string second_operand_str;
	int first_operand_number;
	int second_operand_number;
	std::pair<std::string, int> pair_data;
	std::string base_str;
	int base;
	std::string result_of_instr_str;
	int total_result_num;
	int index_of_assign;
	std::string input_str;
	std::regex regular;
	std::cmatch result_of_reg;
	std::string tmp_str;
	bool found_operation = false;
	int index_of_find = -1;

	for (int i = 0; i < this->value_of_oper_list - 1; i++)
	{
		index_of_find = str.find(this->operations_list[i].first.first);
		if (index_of_find != std::string::npos) //что-то нашлось
		{

			first_operand_str.clear();
			second_operand_str.clear();
			result_of_instr_str.clear();
			base_str.clear();
			tmp_str.clear();

			index_of_assign = str.find(assignment_symbol);
			if (index_of_assign == std::string::npos) //output или error
			{

				if (this->operations_list[i].first.second != "<<")  //(intmp)output;
				{
					continue;
				}

				regular = "([ ]*)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]*)([\\(])([ ]*)([\\w\\-]+)([ ]*)([\\)])([ ]*)";
				if (std::regex_match(str.c_str(), result_of_reg, regular)) // output(what_to_output);
				{
					if (result_of_reg[2] != this->operations_list[i].first.first)
					{
						throw MyException("Wrong output instruction. Or you have forgotten an assignment simbol.");
					}

					found_operation = true;
					first_operand_str = result_of_reg[6];
					first_operand_number = get_num_from_var(first_operand_str);
					base = 10;

					result_of_instr_str = from_decimal_to_base(first_operand_number, base);
					output_data_message(first_operand_str, base, result_of_instr_str);
					break;

				}
				//else
				//a output b;
				regular = "([ ]*)([\\w\\-]+)([ ]+)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]+)([\\w\\-]+)([ ]*)";
				if (std::regex_match(str.c_str(), result_of_reg, regular))
				{
					found_operation = true;
					first_operand_str = result_of_reg[2];
					first_operand_number = get_num_from_var(first_operand_str);

					base_str = result_of_reg[6];
					base = get_base_from_str(base_str);

					result_of_instr_str = from_decimal_to_base(first_operand_number, base);
					output_data_message(first_operand_str, base, result_of_instr_str);
				}
				else
				{
					throw MyException("Wrong output operation syntaxis. Please, try again. May be you have forgotten space symbols.");
				}
			}
			else //значит, что-то нашлось с знаком равно.
			{
				for (int j = index_of_assign + assignment_symbol.length(); j < str.length(); j++)  //(a)input = asd2
				{
					if (!isspace(str[j]))
					{
						if (!isalnum(str[j]))
						{
							throw MyException("Variable can`t consists punctuation symbols.");
						}
						result_of_instr_str += str[j];
					}
				}
				if (isdigit(result_of_instr_str[0]))
				{
					throw MyException("Variable can`t starts with a digit.");
				}
				if (!is_var_not_conflict_with_oper(result_of_instr_str))
				{
					throw MyException("Conflict between operation and variable names.");
				}
				tmp_str = str;
				tmp_str.erase(index_of_assign);

				regular = "([ ]*)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]*)([\\(])([ ]*)([\\w]*)([ ]*)([\\)])([ ]*)"; //Отловим шаблон input
				if (std::regex_match(tmp_str.c_str(), result_of_reg, regular))
				{
					//если нашлось, то это наш input или ошибка.
					if (this->operations_list[i].first.second != ">>") //input(2) = addbar;
					{
						continue;
						//throw MyException("Wrong input operation.");
					}
					if (result_of_reg[2] != this->operations_list[i].first.first)
					{
						continue;
						//throw MyException("Wrong instruction. Regular check error.");
					}


					if (result_of_reg[6].length() != 0) //input(base)=a
					{
						base_str = result_of_reg[6];
						base = get_base_from_str(base_str);

					}
					else //input() = a
					{
						base = 10;
					}

					found_operation = true;
					input_str_message(result_of_instr_str, base);
					std::cin >> input_str;
					total_result_num = from_base_to_decimal(input_str, base);
					pair_data.first = result_of_instr_str;
					pair_data.second = total_result_num;
					auto it = data.find(pair_data.first);
					if (it != data.end())
					{
						data[result_of_instr_str] = total_result_num;
						break;
					}

					data.insert(pair_data);
					break;
				}
				//остальные операции (add, sub, mult и т.д.)
				regular = "([ ]*)([\\w\\-]+)([ ]+)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]+)([\\w\\-]+)([ ]*)";
				if (!std::regex_match(tmp_str.c_str(), result_of_reg, regular)) //нашлась инструкция
				{
					throw MyException("Error syntaxis of your instruction, please, check it out and try again.");
				}

				if (result_of_reg[4] != this->operations_list[i].first.first) // addd
				{
					continue;
				}

				found_operation = true;
				first_operand_str = result_of_reg[2];
				first_operand_number = get_num_from_var(first_operand_str);
				second_operand_str = result_of_reg[6];
				second_operand_number = get_num_from_var(second_operand_str);
				total_result_num = this->operations_list[i].second(first_operand_number, second_operand_number);
				pair_data.first = result_of_instr_str;
				pair_data.second = total_result_num;

				auto it = data.find(pair_data.first);
				if (it != data.end())
				{
					data[result_of_instr_str] = total_result_num;
					break;
				}
				data.insert(pair_data);
				break;
			}
		}

	}
	if (!found_operation)
	{
		throw MyException("Operation not found.");
	}
}

void Interpretator::right_and_op_between_unar_aft(const std::string& str)
{
	std::string first_operand_str;
	std::string second_operand_str;
	int first_operand_number;
	int second_operand_number;
	std::pair<std::string, int> pair_data;
	std::string base_str;
	int base;
	std::string result_of_instr_str;
	int total_result_num;
	int index_of_assign;
	std::string input_str;
	std::regex regular;
	std::cmatch result_of_reg;
	std::string tmp_str;
	bool found_operation = false;
	int index_of_find = -1;

	for (int i = 0; i < this->value_of_oper_list - 1; i++)
	{
		index_of_find = str.find(this->operations_list[i].first.first);
		if (index_of_find != std::string::npos) //что-то нашлось
		{

			first_operand_str.clear();
			second_operand_str.clear();
			result_of_instr_str.clear();
			base_str.clear();
			tmp_str.clear();

			index_of_assign = str.find(assignment_symbol);
			if (index_of_assign == std::string::npos) //output или error
			{

				if (this->operations_list[i].first.second != "<<")  //(intmp)output;
				{
					continue;
				}

				regular = "([ ]*)([\\(])([ ]*)([\\w\\-]+)([ ]*)([\\)])([ ]*)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]*)";
				if (std::regex_match(str.c_str(), result_of_reg, regular)) // output(what_to_output);
				{
					if (result_of_reg[8] != this->operations_list[i].first.first)
					{
						throw MyException("Wrong output instruction. Or you have forgotten an assignment simbol.");
					}

					found_operation = true;
					first_operand_str = result_of_reg[4];
					first_operand_number = get_num_from_var(first_operand_str);
					base = 10;

					result_of_instr_str = from_decimal_to_base(first_operand_number, base);
					output_data_message(first_operand_str, base, result_of_instr_str);
					break;

				}
				//else
				//a output b;
				regular = "([ ]*)([\\w\\-]+)([ ]+)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]+)([\\w\\-]+)([ ]*)";
				if (std::regex_match(str.c_str(), result_of_reg, regular))
				{
					found_operation = true;
					first_operand_str = result_of_reg[2];
					first_operand_number = get_num_from_var(first_operand_str);

					base_str = result_of_reg[6];
					base = get_base_from_str(base_str);

					result_of_instr_str = from_decimal_to_base(first_operand_number, base);
					output_data_message(first_operand_str, base, result_of_instr_str);
				}
				else
				{
					throw MyException("Wrong output operation syntaxis. Please, try again. May be you have forgotten space symbols.");
				}
			}
			else //значит, что-то нашлось с знаком равно.
			{
				for (int j = index_of_assign + assignment_symbol.length(); j < str.length(); j++)  //(a)input = asd2
				{
					if (!isspace(str[j]))
					{
						if (!isalnum(str[j]))
						{
							throw MyException("Variable can`t consists punctuation symbols.");
						}
						result_of_instr_str += str[j];
					}
				}
				if (isdigit(result_of_instr_str[0]))
				{
					throw MyException("Variable can`t starts with a digit.");
				}
				if (!is_var_not_conflict_with_oper(result_of_instr_str))
				{
					throw MyException("Conflict between operation and variable names.");
				}
				tmp_str = str;
				tmp_str.erase(index_of_assign);

				regular = "([ ]*)([\\(])([ ]*)([\\w]*)([ ]*)([\\)])([ ]*)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]*)"; //Отловим шаблон input
				if (std::regex_match(tmp_str.c_str(), result_of_reg, regular))
				{
					//если нашлось, то это наш input или ошибка.
					if (this->operations_list[i].first.second != ">>")
					{
						continue;
					}
					if (result_of_reg[8] != this->operations_list[i].first.first) //inpute != input
					{
						continue;
					}


					if (result_of_reg[4].length() != 0) //(base)input=a
					{
						base_str = result_of_reg[4];
						base = get_base_from_str(base_str);

					}
					else //input() = a
					{
						base = 10;
					}

					found_operation = true;
					input_str_message(result_of_instr_str, base);
					std::cin >> input_str;
					total_result_num = from_base_to_decimal(input_str, base);
					pair_data.first = result_of_instr_str;
					pair_data.second = total_result_num;
					auto it = data.find(pair_data.first);
					if (it != data.end())
					{
						data[result_of_instr_str] = total_result_num;
						break;
					}

					data.insert(pair_data);
					break;
				}
				//остальные операции (add, sub, mult и т.д.)
				regular = "([ ]*)([\\w\\-]+)([ ]+)([\\w\\{\\}\\.\\/+\\-\\)\\`\\?\\(\\*\\=\\&\\%\\$\\<\\>\\,\\№\"\\#\\@\\!\\^]+)([ ]+)([\\w\\-]+)([ ]*)";
				if (!std::regex_match(tmp_str.c_str(), result_of_reg, regular)) //нашлась инструкция
				{
					throw MyException("Error syntaxis of your instruction, please, check it out and try again.");
				}

				if (result_of_reg[4] != this->operations_list[i].first.first) // addd
				{
					continue;
				}

				found_operation = true;
				first_operand_str = result_of_reg[2];
				first_operand_number = get_num_from_var(first_operand_str);
				second_operand_str = result_of_reg[6];
				second_operand_number = get_num_from_var(second_operand_str);
				total_result_num = this->operations_list[i].second(first_operand_number, second_operand_number);
				pair_data.first = result_of_instr_str;
				pair_data.second = total_result_num;

				auto it = data.find(pair_data.first);
				if (it != data.end())
				{
					data[result_of_instr_str] = total_result_num;
					break;
				}
				data.insert(pair_data);
				break;
			}
		}

	}
	if (!found_operation)
	{
		throw MyException("Operation not found.");
	}
}

void Interpretator::parse_and_solve(const std::string& str)
{
	if (this->is_res_left)
	{
		if (this->is_op_before && !this->is_op_between)
		{
			/*
			* perem = op(op1, op2); peremravnoop(op1, op2);
			*
			* output(perem, base); output(perem);
			* peremravnoinput(base);  peremravnoinput();
			*/
			left_and_op_before(str);
		}
		else if (this->is_op_after && !this->is_op_between)
		{
			/*
			* perem = (op1, op2)op; perem=(op1, op2)op;
			*
			* (perem, base)output; (perem)output;
			* perem=(base)input;  perem=()input;
			*/
			left_and_op_after(str);
		}
		else if (this->is_op_before && this->is_op_between)
		{
			/*
			* perem = op1 op op2;;
			* perem output base;
			* output(perem)
			* perem=input(base);  perem=input();
			*/
			left_and_op_between_unar_bef(str);
		}
		else if (this->is_op_after && this->is_op_between)
		{
			/*
			* perem = op1 op op2;;
			* perem output base;
			* (perem)output
			* perem=(base)input;  perem=()input;
			*/
			left_and_op_between_unar_aft(str);
		}
	}
	else //right
	{
		if (this->is_op_before && !this->is_op_between)
		{
			/*
			* op(op1, op2) = perem; op(op1, op2) ravno perem;
			*
			* output(perem, base); output(perem);
			* input(base) = perem;  input() = perem;
			*/
			right_and_op_before(str);
		}
		else if (this->is_op_after && !this->is_op_between)
		{
			/*
			* (op1, op2)op = perem; (op1, op2)op ravno perem;
			*
			* (perem, base)output; (perem)output; ;
			* (base)input = perem;  ()input = perem;
			*/
			right_and_op_after(str);
		}
		else if (this->is_op_before && this->is_op_between)
		{
			/*
			* op1 op op2 = perem;
			* perem output base;
			* output(perem)
			* input(base)=perem; input()=perem;
			*/
			right_and_op_between_unar_bef(str);
		}
		else if (this->is_op_after && this->is_op_between)
		{
			/*
			* op1 op op2 = perem;
			* perem output base;
			* (perem)output
			* (base)input=perem; ()input=perem;
			*/
			right_and_op_between_unar_aft(str);
		}
	}
}


int add(const int first, const int second)
{
	return first + second;
}
int mult(const int first, const int second)
{
	return first * second;
}
int sub(const int first, const int second)
{
	return first - second;
}
int power(const int a, const int n)
{
	unsigned long long int res = 1;
	//unsigned long long int result;
	int tmp_n = n;
	int tmp_a = a;


	if (a == 0)
	{
		return 0;
	}
	else if (n < 0)
	{
		throw MyException("Second operand of pow couldn`t be less than 0.");
	}

	while (tmp_n)
	{
		if (tmp_n & 1)
		{
			res *= tmp_a;
			if (res > INT_MAX)
			{
				throw MyException("Int data type overflow.");
			}
			--tmp_n;
		}
		else
		{
			tmp_a *= tmp_a;
			tmp_n >>= 1;
		}
	}
	return res;
}
int divis(const int first, const int second)
{
	if (second == 0)
	{
		throw MyException("Zero division error.");
	}
	return first / second;
}
int rem(const int first, const int second)
{
	if (second == 0)
	{
		throw MyException("Zero division error.");
	}
	return first % second;
}
int my_xor(const int first, const int second)
{
	return first ^ second;
}
void Interpretator::init_operations() //Лист операций - это пара. Первый элемент - пара из синонима и операции непосредственно. Второй - указатель на функцию операции.
{
	operations_list[0].second = add;
	operations_list[0].first.second = "+";
	operations_list[1].second = mult;
	operations_list[1].first.second = "*";
	operations_list[2].second = sub;
	operations_list[2].first.second = "-";
	operations_list[3].second = power;
	operations_list[3].first.second = "**";
	operations_list[4].second = divis;
	operations_list[4].first.second = "/";
	operations_list[5].second = rem;
	operations_list[5].first.second = "%";
	operations_list[6].second = my_xor;
	operations_list[6].first.second = "^";
	//operations_list[7].second = input;
	operations_list[7].first.second = ">>";
	//operations_list[8].second = output;
	operations_list[8].first.second = "<<";

	operations_list[9].first.second = "=";
}


int main(int argc, char* argv[]) //settings ---- instr_file
{
	setlocale(LC_ALL, "");
	std::string settings_file_name;
	std::string instructions_file_name;
	std::cout << yellow << "---------------------------------------------------------------------------" << white << std::endl;
	std::cout << yellow << "| Interpretator was made by: " << blue << "Kulikov Pavel.  Group: M80-211B-20. MAI-FIIT " << yellow << "|" << white << std::endl;
	std::cout << yellow << "---------------------------------------------------------------------------" << white << std::endl;

	if (argc == 1)
	{
		std::cout << red << "There are no arguments in argv." << white << std::endl;
		return -1;
	}
	if (argc == 2) //only instructions file
	{
		instructions_file_name = argv[1];
	}
	else if (argc == 3) // settings + instructions
	{
		settings_file_name = argv[1];
		instructions_file_name = argv[2];
	}
	else if (argc > 3)
	{
		std::cout << red << "There are too many arguments in argv." << white << std::endl;
		return -1;
	}
	try
	{
		Interpretator interpretator;
		if (argc == 2)
		{
			interpretator.set_settings();
		}
		else if (argc == 3)
		{
			interpretator.set_settings(settings_file_name.c_str());
		}

		interpretator.interpretate_instr(instructions_file_name.c_str()); //TODO!
	}
	catch (MyException& err)
	{
		std::cout << "Interpretate status: " << red << "Error." << white << std::endl;
		std::cout << red << err.what_str() << white << std::endl;
		return -1;
	}

	return 0;
}
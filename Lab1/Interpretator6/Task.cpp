#include <iostream>
#include "Colours.h"
#include<fstream>
#include<cctype>
#include <string>

class MyException : public std::exception
{
private:
	const char* message;
public:
	MyException(const char* message)
	{
		this->message = message;
	}
	const char* what() const override
	{
		return message;
	}
};

int binary_search_pairs(std::pair<std::string, int(*)(unsigned const int, unsigned const int)>* pairs_arr, const std::string what_to_find, int length_arr);
void sort_pairs(std::pair<std::string, int(*)(unsigned const int, unsigned const int)>*, int length);
int from_base_to_decimal(const std::string& num, const int base);
std::string from_decimal_to_base(int value, int base);
const char what_is_the_instruction(const std::string instruction);
bool is_numeric_system_correct(const std::string num, const int base);

int disjunction(unsigned const int first, unsigned const int second);
int conjunction(unsigned const int first, unsigned const int second);
int implication(unsigned const int first, unsigned const int second);
int reverse_implication(unsigned const int first, unsigned const int second);
int equivalention(unsigned const int first, unsigned const int second);
int add_mod_2(unsigned const int first, unsigned const int second);
int ko_implication(unsigned const int first, unsigned const int second);
int sheffer(unsigned const int first, unsigned const int second);
int pirse(unsigned const int first, unsigned const int second);
int negativation(unsigned const int first, unsigned const int second);

int from_base_to_decimal(const std::string& num, const int base)
{
	if (!is_numeric_system_correct(num, base))
	{
		throw MyException("There is error in your numeric system and number.");
	}
	unsigned int result = 0;
	for (int i = 0; i < num.length(); i++)
	{
		result = result * base + (isdigit(num[i]) ? num[i] - '0' : toupper(num[i]) - 'A' + 10);
	}
	return result;
}

std::string from_decimal_to_base(int value, int base)
{
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

const char what_is_the_instruction(const std::string instruction)
{
	char result_of_func = '\0';
	int found;
	found = instruction.find("READ");
	if (found != std::string::npos) //Found READ
	{
		if (found != 0)
		{
			throw MyException("Bad instruction with <READ>. Check your instruction again.");
		}
		return 'r';
	}


	found = instruction.find("WRITE");
	if (found != std::string::npos) //Found READ
	{
		if (found != 0)
		{
			throw MyException("Bad instruction with <WRITE>. Check your instruction again.");
		}
		return 'w';
	}

	if (isalpha(instruction[0]) && instruction[1] == ':' && instruction[2] == '=' && (instruction[3] == '\\' || isalpha(instruction[3])))
	{
		if (instruction[3] == '\\')
		{
			if (instruction.size() != 5 || !isalpha(instruction[4]))
			{
				throw MyException("Bad syntaxis with <\\> operation. Check your instruction and try again.");
			}
		}
		return 'o';
	}

	return result_of_func;
}

void sort_pairs(std::pair<std::string, int(*)(unsigned const int, unsigned const int)>* pairs_arr, int length)
{
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < length - i - 1; j++)
		{
			if (strcmp(pairs_arr[j].first.c_str(), pairs_arr[j + 1].first.c_str()) > 0)
			{
				std::swap(pairs_arr[j], pairs_arr[j + 1]);
			}
		}
	}
}
int compare_pairs(const void* first, const void* second)
{
	if (first > second)
	{
		return 1;
	}
	else if (first < second)
	{
		return -1;
	}
	else return 0;
}
int binary_search_pairs(std::pair<std::string, int(*)(unsigned const int, unsigned const int)>* pairs_arr, const std::string what_to_find, int length_arr)
{
	int left_wall = 0;
	int right_wall = length_arr - 1;
	int need_index = -1;
	int middle = -1;
	int cmp_res;
	while (left_wall <= right_wall)
	{
		middle = (right_wall + left_wall) / 2;
		cmp_res = strcmp(what_to_find.c_str(), pairs_arr[middle].first.c_str());
		if (cmp_res > 0)
		{
			left_wall = middle + 1;
		}
		else if (cmp_res < 0)
		{
			right_wall = middle - 1;
		}
		else
		{
			need_index = middle;
			break;
		}
	}
	return need_index;
}

int disjunction(unsigned const int first, unsigned const int second)
{
	return first | second;
}
int conjunction(unsigned const int first, unsigned const int second)
{
	return first & second;
}
int implication(unsigned const int first, unsigned const int second)
{
	return (~first) | second;
}
int reverse_implication(unsigned const int first, unsigned const int second)
{
	return first | (~second);
}
int equivalention(unsigned const int first, unsigned const int second)
{
	return ~add_mod_2(first, second);
}
int add_mod_2(unsigned const int first, unsigned const int second)
{
	return first ^ second;
}
int ko_implication(unsigned const int first, unsigned const int second)
{
	return ~implication(first, second);
}
int sheffer(unsigned const int first, unsigned const int second)
{
	return ~conjunction(first, second);
}
int pirse(unsigned const int first, unsigned const int second)
{
	return ~disjunction(first, second);
}
int negativation(unsigned const int first, unsigned const int second) //second unused
{
	return ~first;
}


int main(int argc, char* argv[])
{
	std::cout << azure << "INTERPRETATOR was made by" << blue << " Kulikov Pavel. " << azure << "Group: " << blue << "M80-211B-20" << white << std::endl;
	if (argc == 1)
	{
		std::cout << red << "There is a mistake with value of arguments." << white << std::endl;
		return -1;
	}
	std::ofstream file_write;
	int num_of_instr = 0;
	bool trace_flag = false;
	if (argc == 4)
	{
		if (strcmp(argv[2], "\\trace") != 0)
		{
			std::cout << red << "There is a mistake with arguments. Error with: " << argv[2] << white << std::endl;
			return -1;
		}
		trace_flag = true;
		file_write.open(argv[3]);
	}

	std::ifstream file_read;
	file_read.open(argv[1], std::ios::in);
	
	if (!file_read.is_open())
	{
		std::cout << red <<"There is a mistake with file openning." << white <<  std::endl;
		if (argc == 4)
		{
			file_write.close();
		}
		return -1;
	}
	const int size_data_arr = 26;
	const int val_of_opers = 10;
	int data_nums[size_data_arr];
	int* ptrs_on_data_nums[size_data_arr];
	std::pair<std::string, int(*)(unsigned const int, unsigned const int)> operations_with_funcs[val_of_opers];
	operations_with_funcs[0].first = "+";
	operations_with_funcs[0].second = disjunction;
	operations_with_funcs[1].first = "&";
	operations_with_funcs[1].second = conjunction;
	operations_with_funcs[2].first = "->";
	operations_with_funcs[2].second = implication;
	operations_with_funcs[3].first = "<-";
	operations_with_funcs[3].second = reverse_implication;
	operations_with_funcs[4].first = "~";
	operations_with_funcs[4].second = equivalention;
	operations_with_funcs[5].first = "<>";
	operations_with_funcs[5].second = add_mod_2;
	operations_with_funcs[6].first = "+>";
	operations_with_funcs[6].second = ko_implication;
	operations_with_funcs[7].first = "?";
	operations_with_funcs[7].second = sheffer;
	operations_with_funcs[8].first = "!";
	operations_with_funcs[8].second = pirse;
	operations_with_funcs[9].first = "\\";
	operations_with_funcs[9].second = negativation;

	/*std::qsort(operations_with_funcs, val_of_opers, sizeof(std::string), compare_pairs);
	for (int i = 0; i < val_of_opers; i++)
	{
		std::cout << green << operations_with_funcs[i].first << white << std::endl;
	}*/
	sort_pairs(operations_with_funcs, val_of_opers);
	for (int i = 0; i < val_of_opers; i++)
	{
		std::cout << green << operations_with_funcs[i].first << white << std::endl;
	}

	for (int i = 0; i < size_data_arr; i++)
	{
		data_nums[i] = -1;
		ptrs_on_data_nums[i] = &data_nums[i];
	}

	int in_comment = 0;
	std::string instruction = "";
	char symbol;

	char instr_checker;

	int base = 0;
	char* base_ptr = nullptr;
	std::string base_str;

	char first_operand;
	char second_operand;
	int first_index_operand = -1;
	int second_index_operand = -1;

	char result;
	int result_index = -1;

	std::string operation;
	int operation_index = -1;
	char* help_ptr = nullptr;
	
	std::string user_str;
	std::string output_data;
	try
	{
		while (!file_read.eof())
		{
			symbol = (char)file_read.get();
			if (isspace(symbol))
			{
				continue;
			}

			if (symbol == '%') //oneline comment.
			{
				while (!file_read.eof() && symbol != '\n')
				{
					symbol = (char)file_read.get();  //skip everything before the '\r'.
					
				}
				continue; //skip '\r' and \n.
			}
			else if (symbol == '}' && !in_comment)
			{
				throw MyException("Was found closed bracket without open bracket.");
			}
			else if (symbol == '{' || in_comment)
			{
				if (symbol == '{')
				{
					in_comment++;
				}

				while (!file_read.eof() && symbol != '}')
				{
					symbol = (char)file_read.get();

					if(symbol == '{')
					{
						in_comment++;
					}
				}

				if (symbol == '}')
				{
					in_comment--;
				}
				continue;
			}
			else if (symbol == '}')
			{
				in_comment--;
				continue;
			}

			if (!in_comment && symbol == ';')// (probably) GOOD INSTRUCTION. PARSING INSTRUCTION.
			{
				if (trace_flag)
				{
					num_of_instr++;
					file_write << "----------------------------" << std::endl << num_of_instr << std::endl;
					file_write << instruction << std::endl;
				}

				instr_checker = what_is_the_instruction(instruction);
				if (!instr_checker)
				{
					throw MyException("Bad instruction. Check it out and try again.");
				}
				
				switch (instr_checker)
				{
					case 'r': //READ.
					{
						if (instruction[4] != '(' || !isalpha(instruction[5]) || instruction[6] != ',')
						{
							throw MyException("Bad instruction! Check your instruction and try again.");
						}

						base_ptr = &instruction[7]; //7 - first probably digit in read instruction.
						base_str = "";
						while (isdigit(*base_ptr))
						{
							base_str += *base_ptr;
							base_ptr++;
						}
						if (*base_ptr != ')')
						{
							throw MyException("Bad instruction! Check your instruction and try again.");
						}
						
						first_operand = instruction[5];
						first_index_operand = first_operand - 'A';
						base = from_base_to_decimal(base_str, 10);
						base_str.clear();
						
						std::cout << "Please input a number in " << green << first_operand << white << ". Numeric system: " << green <<  base << white << ". >> ";
						std::cin >> user_str;
						data_nums[first_index_operand] = from_base_to_decimal(user_str, base);
						if (trace_flag)
						{
							file_write << "Was input: " << first_operand << " = " << user_str << "(" << base << ")" << std::endl;
						}
						user_str.clear();
						break;
					}
					case 'w': //WRITE
					{
						if (instruction[5] != '(' || !isalpha(instruction[6]) || instruction[7] != ',')
						{
							throw MyException("Bad instruction! Check your instruction and try again.");
						}
						base_ptr = &instruction[8]; //8 - first probably digit in write instruction.
						base_str = "";
						while (isdigit(*base_ptr))
						{
							base_str += *base_ptr;
							base_ptr++;
						}
						if (*base_ptr != ')')
						{
							throw MyException("Bad instruction! Check your instruction and try again.");
						}

						first_operand = instruction[6];
						first_index_operand = first_operand - 'A';
						base = from_base_to_decimal(base_str, 10);
						base_str.clear();
						if (*ptrs_on_data_nums[first_index_operand] == -1)
						{
							throw MyException("Access violation error. Impossible to read not initialized memory.");
						}
						output_data = from_decimal_to_base(*ptrs_on_data_nums[first_index_operand], base);
						if (output_data.size() == 0)
						{
							output_data += "0";
						}
						if (trace_flag)
						{
							file_write << "Output data from <WRITE>:  " << first_operand << " = " << output_data << "(" \
								       << base << ")" << std::endl;
						}
						std::cout << yellow << "Output data from <WRITE>:  " << white << first_operand << " = " << output_data << "(" \
								  << base << ")" <<  std::endl;
						break;
					}
					case 'o': //operation
					{
						result = instruction[0];
						result_index = result - 'A';
						if (instruction[3] == '\\')
						{

							first_operand = instruction[4];
							first_index_operand = first_operand - 'A';
							operation = "\\";

							if (*ptrs_on_data_nums[first_index_operand] == -1)
							{
								throw MyException("Access violation error. Impossible to read not initialized memory.");
							}
							if (trace_flag)
							{
								file_write << "Your operation: " << operation << std::endl;
								file_write << "First operand: " << first_operand << std::endl;
								file_write << "Result operand: " << result << std::endl;
							}
						}
						else
						{
							first_operand = instruction[3];
							first_index_operand = first_operand - 'A';
							help_ptr = &instruction[4];
							while (ispunct(*help_ptr))
							{
								operation += *help_ptr;
								help_ptr++;
							}
							if (operation.size() > 2 || (operation.size() == 0))
							{
								throw MyException("Operation is not found.");
							}
							

							if (!isalpha(*help_ptr))
							{
								
								throw MyException("Operation error. Problem with the second operand.");
								
							}
							second_operand = *help_ptr;
							second_index_operand = second_operand - 'A';
							
							if (trace_flag)
							{
								file_write << "Your operation: " << operation << std::endl;
								file_write << "First operand: " << first_operand << std::endl;
								file_write << "Second operand: " << second_operand << std::endl;
								file_write << "Result operand: " << result << std::endl;
							}

							if (*ptrs_on_data_nums[first_index_operand] == -1)
							{
								throw MyException("Access violation error. Impossible to read not initialized memory.");
							}

							if (*ptrs_on_data_nums[second_index_operand] == -1)
							{
								
								throw MyException("Access violation error. Impossible to read not initialized memory.");
							}
							
						}
						operation_index = binary_search_pairs(operations_with_funcs, operation, val_of_opers);
						if (operation_index == -1)
						{
							
							throw MyException("Operation not found!");
							std::cout << red << "Operation not found!" << white << std::endl;
							
						}


						//IMPORTANT
						if (operation == "\\")
						{
							(*ptrs_on_data_nums[result_index]) = abs(operations_with_funcs[operation_index].second(*ptrs_on_data_nums[first_index_operand], 0));
						}
						else
						{
							(*ptrs_on_data_nums[result_index]) = abs(operations_with_funcs[operation_index].second(*ptrs_on_data_nums[first_index_operand], *ptrs_on_data_nums[second_index_operand]));
						}
						//IMPORTANT
						if (trace_flag)
						{
							file_write << "Data was successfully inserted." << std::endl;
						}

						operation.clear();
						break;
					}
					default:
					{
						
						throw MyException("Bad instruction! Check your instruction and try again.");
						
					}
				}
				
				instruction.clear();
				continue;
			}
			

			instruction += (isalpha(symbol) ? toupper(symbol): symbol);
		}


		if (file_read.eof() && in_comment)
		{
			
			throw MyException("There is bracket value mistake. Closed bracket is not found.");
		}
	}
	catch (MyException& err)
	{
		if (trace_flag)
		{
			file_write << err.what() << " Error on <<" << num_of_instr << ">> line." << std::endl;
			file_write << "----------------------------" << std::endl;
		}
		std::cout << red << err.what() << white << std::endl;
		file_read.close();
		if (argc == 4)
		{
			file_write.close();
		}
		return -2;
	}

	
	file_read.close();
	
	
	if (argc == 4)
	{
		file_write << "----------------------------" << std::endl;
		file_write.close();
	}

	return 0;
}
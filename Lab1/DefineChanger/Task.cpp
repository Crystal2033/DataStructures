#include <iostream>
#include "Colours.h"
#include<fstream>
#include<cctype>
#include <string>
#include<list>
#define HASHSIZE 128
#define BASE_SYS 62


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

unsigned long long from_base_to_decimal(const std::string& word, const int base= BASE_SYS)
{
	
	unsigned long long result = 0;
	unsigned long long summand = 0;
	for (int i = 0; i < word.length(); i++)
	{

		if (!isalnum(word[i]))
		{
			throw MyException("Wrong symbol in word. Word could consists digits of alphas.");
		}
		if (isdigit(word[i]))
		{
			summand = word[i] - '0';
		}
		else if (isupper(word[i]))
		{
			summand = word[i] - 'A' + 10;
		}
		else
		{
			summand = word[i] - 'a' + 36;
		}

		result = result * base + summand;
	}
	return result;
	
}

class HashTable 
{
private:
	std::list<std::pair<std::string, std::string>> arr_list[HASHSIZE];
	int get_hash(const std::string& word) const;
public:

	void push_pair(const std::pair<std::string, std::string>&);
	std::string find_word(const std::string& word) const;
};

int HashTable::get_hash(const std::string& word) const
{
	unsigned long long key = 0;
	int hash = -1;
	key = from_base_to_decimal(word);
	hash = key % HASHSIZE;

	return hash;
}

void HashTable::push_pair(const std::pair<std::string, std::string>& pair)
{
	int index = get_hash(pair.first);
	auto it = arr_list[index].begin();
	if (arr_list[index].size() == 0)
	{
		arr_list[index].push_back(pair);
	}
	else
	{
		while (it != arr_list[index].end())
		{
			if ((*it).first == pair.first) //the simillar words
			{
				return;
			}
		}
		arr_list[index].push_back(pair);
	}
}

std::string HashTable::find_word(const std::string& word) const
{
	int index = get_hash(word);
	auto it = arr_list[index].begin();
	if (arr_list[index].size() == 0)
	{
		return word;
	}
	else
	{
		while (it != arr_list[index].end())
		{
			if ((*it).first == word)
			{
				return (*it).second;
			}
			it++;
		}
		return word;
	}

}

int main(int argc, char* argv[])
{
	std::cout << azure << "DefineChanger was made by" << blue << " Kulikov Pavel. " << azure << "Group: " << blue << "M80-211B-20" << white << std::endl;
	std::ifstream file_read;
	std::ofstream file_output;
	const std::string file_name = "Output.txt";
	char symbol;
	int checker = -1;
	const std::string const_define_str = "define";
	int start_def_name_index = -1;
	int start_def_val_index = -1;
	std::string check_word;
	bool word_has_punct_symb = false;
	bool is_define = false;
	HashTable hash_table;

	try
	{
		file_read.open(argv[1]);
		if (!file_read.is_open())
		{
			std::cout << red <<  "File was for reading not open." << white << std::endl;
			return -1;
		}

		file_output.open(file_name);
		if (!file_output.is_open())
		{
			std::cout << red << "File for writing was not open." << white << std::endl;
			return -1;
		}


		while (!file_read.eof())
		{
			checker = -1;
			std::string def_name = "";
			std::string def_value = "";
			std::string define_line = "";
			
			symbol = file_read.get();

			if (symbol == '#') //define instr
			{
				while (symbol != '\n' && symbol != EOF)
				{
					define_line += symbol;
					symbol = file_read.get();
				}
				checker = define_line.find("define");
				if (checker == std::string::npos)
				{
					throw MyException("Wrong syntaxis define!");
				}
				
				start_def_name_index = checker + const_define_str.size();
				if (define_line[start_def_name_index] != ' ') //#defineABCD != #define
				{
					file_output << define_line;
					file_output << symbol;
					continue;
				}

				int j = start_def_name_index + 1;

				while (isspace(define_line[j]))
				{
					j++;
					continue;
				}
				while (!isspace(define_line[j]))
				{
					def_name += define_line[j];
					j++;
				}

				while (isspace(define_line[j]))
				{
					j++;
					continue;
				}
				while (!isspace(define_line[j]) && define_line[j])
				{
					def_value += define_line[j];
					j++;
				}
				auto created_pair = std::make_pair(def_name, def_value);
				hash_table.push_pair(created_pair);
			}
			else //good text
			{

				if (isspace(symbol) && !check_word.empty())
				{
					if (!word_has_punct_symb)
					{
						file_output << hash_table.find_word(check_word);
						file_output << symbol;
						check_word.clear();
						continue;
					}
					file_output << check_word;
					file_output << symbol;
					word_has_punct_symb = false;
					check_word.clear();
				}
				else if (!isspace(symbol) && !file_read.eof())
				{
					if (ispunct(symbol))
					{
						word_has_punct_symb = true;
					}
					check_word += symbol;
				}
				else if (file_read.eof())
				{
					file_output << check_word;
					check_word.clear();
				}
				else
				{
					file_output << symbol;
				}
			
			}

		}

		file_read.close();
		file_output.close();

		file_read.open(file_name);
		if (!file_read.is_open())
		{
			std::cout << red << "File open error" << white << std::endl;
			return -1;
		}

		file_output.open(argv[1]);
		if (!file_output.is_open())
		{
			std::cout << red << "File open error" << white << std::endl;
			return -1;
		}

		while ((symbol = file_read.get()) != EOF)
		{
			file_output << symbol;
		}

	}
	catch (MyException& err)
	{
		std::cout << red << err.what() << white << std::endl;
		file_read.close();
		file_output.close();
		return -1;
	}
	
	return 0;
}
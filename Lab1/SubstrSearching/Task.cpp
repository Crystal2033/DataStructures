#include <iostream>
#include <stdarg.h>
#include <vector>
#include <fstream>
#include <string>
#include "Colours.h"

class MyException : public std::exception
{
private:
	std::string error_message;
public:
	MyException(const std::string& error)
	{
		this->error_message = error;
	}
	const char* what() const override final
	{
		return error_message.c_str();
	}
};

bool my_strstr(const std::string substring, const std::string text);
std::vector<std::string> get_filenames_with_substr(bool (*foo_substr_finder)(const std::string, const std::string), const std::string substr, const int value_of_file_names, ...);


bool my_strstr(const std::string substring, const std::string text)
{
	if (text.length() < substring.length())
	{
		return false;
	}

	int value_of_similiarity = 0; //если число сходства равно числу символов в подстроке => подстрока найдена.
	for (int i = 0; i < text.length(); i++)
	{

		if (substring[0] == text[i])
		{
			int k = i + 1;  //первая буква уже проверена, при помощи if сверху. k по тексту, j -- по подстроке.
			value_of_similiarity++;

			for (int j = 1; j < substring.length(); j++) //первая буква подстроки уже проверена.
			{
				if ((int)substring[j] == (int)text[k])
				{
					value_of_similiarity++;
					k++;
				}
			}

			if (value_of_similiarity == substring.length())
			{
				return true;
			}
			else
			{
				value_of_similiarity = 0;
			}
		}
	}

	return false;
}
std::vector<std::string> get_filenames_with_substr(bool (*foo_substr_finder)(const std::string, const std::string), const std::string substr, const int value_of_file_names, ...)
{
	va_list arg_list;
	va_start(arg_list, value_of_file_names);

	std::vector<std::string> file_names_vector;
	std::ifstream fout;
	std::string text_file;
	char symbol;
	std::string file_name;

	for (int i = 0; i < value_of_file_names; i++)
	{
		file_name = va_arg(arg_list, const char*);
		fout.open(file_name, std::ios::binary);
		if (!fout.is_open())
		{
			throw MyException("One of your file doesn`t exist. Please, check file names and try again.");
		}
		while (fout.read((char*)&symbol, sizeof(symbol)))
		{
			text_file += symbol;
		}
		if (foo_substr_finder(substr, text_file))
		{
			file_names_vector.push_back(file_name);
		}

		fout.close();
		text_file.clear();
		file_name.clear();
	}
	va_end(arg_list);

	return file_names_vector;
}
int main(void)
{
	std::cout << blue << "Hello, my name is Kulikov Pavel, group: " << azure << "M80-211B-20." << white << std::endl;
	std::cout << blue << "This program is able to find file names which contain your string." << white << std::endl;
	std::vector<std::string> total_file_names;
	
	try
	{
		total_file_names = get_filenames_with_substr(my_strstr, "od\r\n", 3, "TextTest.txt", "SubStringChecker1.txt", "HiComprendo.txt");
	}
	catch (MyException& ex)
	{
		std::cout << red << ex.what() << white << std::endl;
		return -1;
	}
	for (int i = 0; i < total_file_names.size(); i++)
	{
		std::cout << yellow << i + 1 << ". " << green <<  total_file_names[i] << white <<  std::endl;
	}
	if (total_file_names.size() == 0)
	{
		std::cout << red <<"There are no files with your string. Please try again." << white << std::endl;
	}
	return 0;
}
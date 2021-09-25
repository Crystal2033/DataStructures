#include <iostream>
#include <fstream>
#include "Colours.h"
#include <cctype>
#include <String>
#include <vector>
unsigned int transform_to_decimal(const std::string& num, const int base);

unsigned int transform_to_decimal(const std::string& num, const int base)
{
	unsigned int result = 0;
	for (int i = 0; i < num.length(); i++)
	{
		result = result * base + (isdigit(num[i]) ? num[i] - '0' : num[i] - 'A' + 10);
	}
	return result;
}

int main(int argc, char* argv[])
{
	std::cout << yellow << "-----------------------------------------------------------------------------------------------------------" << white << std::endl;
	std::cout << blue << "Hello, my name is Kulikov Pavel, group:" << cyan << " M80-211B-20." << std::endl;
	std::cout << blue << "This program can find minimal numeration system and transform it into decimal system." << white << std::endl;
	std::cout << yellow << "-----------------------------------------------------------------------------------------------------------" << white << std::endl;

	const std::string file_name = argv[1];
	std::ifstream f_read;

	f_read.open(file_name);
	if (!f_read.is_open())
	{
		std::cout << red << "Problem to open your file: " << cyan << white << "<<" << cyan << file_name << white << ">>"<< red <<  ", please, check your file name and try again." << white << std::endl;
		return -1;
	}

	char symbol;
	std::string number_str = "";
	std::vector<std::string> nums_from_file;
	std::vector<int> systems_of_numbers_from_file;
	std::vector<int> numbers_from_file_in_decimal;
	
	while (!f_read.eof())
	{
		symbol = f_read.get();

		if (!isspace(symbol) && symbol != -1) //отлавливалс€ eof. (-1) €. „тобы его не было, пришлось добавить условие. 
		{
			if (!isalnum(symbol))
			{
				std::cout << red << "One of your numbers has incorrect symbol. Please, check it out and try again." << white << std::endl;
				return -2;
			}
			number_str += toupper((char)symbol);
		}
		else
		{
			if (number_str.empty())
			{
				continue;
			}
			nums_from_file.push_back(number_str);
			number_str.clear();
			
		}
	}
	if (!number_str.empty())
	{
		nums_from_file.push_back(number_str);
		number_str.clear();
	}
	
	f_read.close();
	


	const std::string full_symbols_of_all_sys = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	for (int i = 0; i < nums_from_file.size(); i++)
	{
		int tmp_num_sys = 0;
		int min_num_sys = 0;
		for (int j = 0; j < nums_from_file[i].length(); j++)
		{
			tmp_num_sys = full_symbols_of_all_sys.find(nums_from_file[i][j]) + 1;
			if (tmp_num_sys > min_num_sys)
			{
				min_num_sys = tmp_num_sys;
			}
		}
		systems_of_numbers_from_file.push_back(min_num_sys);
	}
	
	for (int i = 0; i < nums_from_file.size(); i++)
	{
		numbers_from_file_in_decimal.push_back(transform_to_decimal(nums_from_file[i], systems_of_numbers_from_file[i]));
	}
	
	std::ofstream f_write;
	const std::string output_file_name = "ResultsNumerationSystems.txt";
	f_write.open(output_file_name, std::ios::trunc); //trunc - удаление старого содержимого файла.
	if (!f_write.is_open())
	{
		std::cout << red << "Problem to open your file: " << cyan << file_name << ", please, check your file name and try again." << white << std::endl;
		return -1;
	}
	for (int i = 0; i < nums_from_file.size(); i++)
	{
		f_write << nums_from_file[i] << "(" << systems_of_numbers_from_file[i] << ")"  << " = " << numbers_from_file_in_decimal[i] << "(10)" << std::endl;
	}

	f_write.close();
	std::cout << green << "-----------------------------------------------------------------------------------------------------------" << white << std::endl;
	std::cout << green << "Success!" << white << std::endl;
	std::cout << green << "-----------------------------------------------------------------------------------------------------------" << white << std::endl;
	std::cout << azure << "Check results in the file: " << white << " <<" << cyan << output_file_name << white << ">>." << std::endl;
	std::cout << yellow << "-----------------------------------------------------------------------------------------------------------" << white << std::endl;

	return 0;
}
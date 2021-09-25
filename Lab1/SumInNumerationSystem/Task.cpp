#include <iostream>
#include "Colours.h"
#include <stdarg.h>
#include <string>
#include <cctype>

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

std::string summ_in_sys(const int base, const int value_of_nums, ...);

std::string summ_in_sys(const int base, const int value_of_nums, ...)
{
	if (value_of_nums == 0)
	{
		throw(MyException("You haven`t input any number, please, try again."));
	}
	

	va_list arg_list;
	va_start(arg_list, value_of_nums);
	std::string first_number;
	std::string second_number;
	std::string result = "";
	int length_first_number = 0, length_second_number = 0;
	const std::string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int need_symbol_index = 0;
	int help_tmp_for_add_one;


	
	first_number = va_arg(arg_list, const char*); //ѕрисваиваем в результат сразу первое слагаемое
	if (value_of_nums == 1)
	{
		return first_number;
	}

	
	for (int i = 0; i < value_of_nums - 1; i++)
	{
		bool need_to_add_one = false;
		second_number = va_arg(arg_list, const char*);
		length_second_number = second_number.length();
		length_first_number = first_number.length();
		result.clear(); // ќчищаем result, чтобы потом с ним работать. ≈сли пройтись разок по циклу, то станет €сно, почему нужно очищать result.

		if (length_second_number < length_first_number) //выравниваем числа по длине.
		{
			for (int j = 0; j < length_first_number - length_second_number ; j++)
			{
				second_number = "0" + second_number;
			}
		}
		else if (length_second_number > length_first_number)
		{
			for (int j = 0; j < length_second_number - length_first_number; j++)
			{
				first_number = "0" + first_number;
			}
		}

		length_first_number = first_number.length(); //¬се равно, какую переменную брать, предположим, это будет result.
		/*for (int k = 0; k < first_number.length(); k++)
		{
			first_number[k] = toupper(first_number[k]);
		}
		for (int k = 0; k < second_number.length(); k++)
		{
			second_number[k] = toupper(second_number[k]);
		}*/

		//„исла выровнены.

		for (int j = length_first_number; j > 0; j--) // идем с конца числа.
		{
			help_tmp_for_add_one = (isdigit(first_number[j - 1]) ? first_number[j - 1] - '0' : toupper(first_number[j - 1]) - 'A' + 10) +
				                   (isdigit(second_number[j - 1]) ? second_number[j - 1] - '0' : toupper(second_number[j - 1]) - 'A' + 10)
				                   + need_to_add_one;

			if (help_tmp_for_add_one >= base) //ѕеременна€ запоминани€ единички, она нам нужна будет дл€ следующего j.
			{
				need_to_add_one = true;
			}
			else
			{
				need_to_add_one = false;
			}

			need_symbol_index = help_tmp_for_add_one % base;
			
			result = alphabet[need_symbol_index] + result;

		}
		if (need_to_add_one)
		{
			result = '1' + result; //ѕриписываем оставшуюс€ единичку, если у нас добавление '1' выходит за пределы числа.
		}
		first_number = result;
		
	}
	
	va_end(arg_list);
	result.erase(0, result.find_first_not_of('0'));

	return result;
}

int main(void)
{
	std::cout << blue <<"Hello, my name is Kulikov Pavel, group:" << azure << " M80-211B-20." << white << std::endl;
	std::cout << blue << "This program is able to summ your numbers in numeration system you chose." << white << std::endl;
	std::cout << yellow << "-------------------------------------------------------------------------" << white << std::endl;
	std::string result;
	try
	{
		result = summ_in_sys(16, 3, "f123f", "F", "1325");
	}
	catch (MyException& ex)
	{
		std::cout << red << ex.what() << white << std::endl;
	}
	std::cout << yellow << "The result of addition is: "<<  green << result << white << std::endl;
	std::cout << yellow << "-------------------------------------------------------------------------" << white << std::endl;
	return 0;
}
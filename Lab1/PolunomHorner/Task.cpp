#include <iostream>
#include "Colours.h"
#include <vector>
#include <stdarg.h>

double solve_polynom(const double x, const int degree_of_equation, ...);
void print_polynom(std::vector<double> coeffs)
{
	std::cout << azure << "f(x) = ";
	for (int i = 0; i < coeffs.size(); i++)
	{
		if (coeffs[i] > 0 && i != 0)
		{
			std::cout << " + ";
		}
		std::cout << coeffs[i];
		if (i < coeffs.size() - 2) // -2, так как не надо писать степень у нулевой и первой степени полинома.
		{
			std::cout << "x^" << coeffs.size() - i - 1;
			continue;
		}
		else if (i == coeffs.size() - 2)
		{
			std::cout << "x";
		}
	}
	std::cout << white << std::endl;
}
double solve_polynom(const double x, const int degree_of_equation, ...)
{
	va_list arg_list;
	va_start(arg_list, degree_of_equation);

	std::vector<double> coeffs;
	for (int i = 0; i < degree_of_equation + 1; i++)
	{
		coeffs.push_back(va_arg(arg_list, const double));
	}
	print_polynom(coeffs);
	std::cout << "X = " << green << x << white << std::endl;
	std::cout << red << "--------------------------------------------------------------" << white << std::endl;
	va_end(arg_list);

	double result = coeffs[0];
	for (int i = 1; i < degree_of_equation + 1; i++)
	{
		result = result * x + coeffs[i];
	}

	return result;
}



int main(void)
{
	std::cout << yellow << "-----------------------------------------------------------------------------------------------------------" << white << std::endl;
	std::cout << blue << "Hello, my name is Kulikov Pavel, group:" << cyan << " M80-211B-20" << blue << " and this program can solve polynom with concrete point." << white << std::endl;
	std::cout << yellow << "-----------------------------------------------------------------------------------------------------------" << white << std::endl;

	std::cout << "Answer: " << green << solve_polynom(2.5, 3, 4.0, 3.0, 5.0, 6.25) << white << std::endl;
	std::cout << red << "--------------------------------------------------------------" << white << std::endl;


	return 0;
}
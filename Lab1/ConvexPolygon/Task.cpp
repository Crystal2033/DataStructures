#include <iostream>
#include "Colours.h"
#include <vector>
#include <stdarg.h>




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

typedef struct Point
{
	double x;
	double y;
}Point;

typedef struct Coord_Vector
{
	double i_ort;
	double j_ort;
}Coord_Vector;

bool is_convex(const int value_of_points, ...);
bool is_repeated(const std::vector<Point>& points, const Point& curr_point);

bool is_repeated(const std::vector<Point>& points, const Point& curr_point)
{
	for (int i = 0; i < points.size(); i++)
	{
		if ((points[i].x == curr_point.x) && (points[i].y == curr_point.y))
		{
			return true;
		}
	}
	return false;
}
bool is_convex(const int value_of_points, ...)
{
	if (value_of_points <= 2)
	{
		throw MyException("Not enough points to make a polygon.");
	}
	va_list arg_list;
	va_start(arg_list, value_of_points);
	std::vector <Point> points_vector;
	Point tmp_point;
	for (int i = 0; i < value_of_points; i++)
	{
		tmp_point = va_arg(arg_list, const Point);
		if (!is_repeated(points_vector, tmp_point))
		{
			points_vector.push_back(tmp_point);
		}
		else
		{
			throw MyException("There are two simillar points, please check your input data and try again.");
		}

	}

	std::cout << azure << "Points:" << white << std::endl;
	for (int i = 0; i < points_vector.size(); i++)
	{
		std::cout << yellow << "(" << points_vector[i].x << ", " << points_vector[i].y << ")" << white <<std::endl;
	}
	std::cout << yellow << "-------------------------------------------------------------------------------------------------------------" << white << std::endl;


	va_end(arg_list);
	std::vector<Coord_Vector> vectors;
	Coord_Vector tmp_vec_coords;

	for (int i = 0; i < value_of_points; i++)
	{
		tmp_vec_coords.i_ort = points_vector[(i + 1) % value_of_points].x - points_vector[i].x;
		tmp_vec_coords.j_ort = points_vector[(i + 1) % value_of_points].y - points_vector[i].y;
		vectors.push_back(tmp_vec_coords);
	}

	double sign_check_tmp = 0.0;
	int value_of_positive_check_tmp = 0;
	int value_of_negative_check_tmp = 0;

	for (int i = 0; i < vectors.size(); i++)
	{
		sign_check_tmp = vectors[i].i_ort * vectors[(i + 1) % value_of_points].j_ort - vectors[i].j_ort * vectors[(i + 1) % value_of_points].i_ort;
		if (sign_check_tmp >= 0)
		{
			value_of_positive_check_tmp++;
		}
		else
		{
			value_of_negative_check_tmp++;
		}
	}

	if (value_of_negative_check_tmp == value_of_points || value_of_positive_check_tmp == value_of_points)
	{
		return true;
	}
	else
	{
		return false;
	}

}

int main(void)
{
	std::cout << yellow << "-------------------------------------------------------------------------------------------------------------" << white << std::endl;
	std::cout << blue << "Hello, my name is Kulikov Pavel, group:" << cyan << " M80-211B-20" << blue << " and this program can define the convexion of the polygon." << white << std::endl;
	Point A, B, C, D; //E
	A.x = 2.0;
	A.y = 5.0;

	B.x = -3.0;
	B.y = 3.0;

	C.x = 0.0;
	C.y = -2.0;

	D.x = 3.0;
	D.y = -1.0;

	/*E.x = 0.0;
	E.y = 2.0;*/
	
	std::cout << yellow << "-------------------------------------------------------------------------------------------------------------" << white << std::endl;
	try
	{

		if (is_convex(4, A, B, C, D))
		{
			std::cout << green << "Your polygon is convex." << white << std::endl;
			
		}
		else
		{
			std::cout << red << "Your polygon is not convex." << white << std::endl;
		}
	}

	catch (const MyException &err)
	{
		std::cout << red << err.what() << white << std::endl;
	}
	std::cout << yellow << "-------------------------------------------------------------------------------------------------------------" << white << std::endl;

	return 0;
}
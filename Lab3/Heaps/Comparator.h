#pragma once
#include<iostream>
#include"Colours.h"

template <typename TKey>
class Comparator
{
public:
	virtual int compare(const TKey&, const TKey&) const = 0;
	virtual ~Comparator() = default;
};


class ComparatorInt final : public Comparator<int>
{
public:
	ComparatorInt() = default;
	int compare(const int& left, const int& right) const override;

};

class ComparatorStr final : public Comparator<std::string>
{
public:

	int compare(const std::string& left, const std::string& right) const override; // по длине строки.
};

class ComparatorDouble final : public Comparator<double> //на всякий случай.
{
public:

	int compare(const double& left, const double& right) const override; // по длине строки.
};

int ComparatorInt::compare(const int& left, const int& right) const
{
	if (left > right)
	{
		return 1;
	}
	else if (left == right)
	{
		return 0;
	}
	else return -1;
}

int ComparatorStr::compare(const std::string& left, const std::string& right) const
{
	if (left.length() > right.length())
	{
		return 1;
	}
	else if (left.length() == right.length())
	{
		return 0;
	}
	else return -1;
}

int ComparatorDouble::compare(const double& left, const double& right) const
{
	double epsilone = 0.0000001;
	if (left - right > epsilone)
	{
		return 1;
	}
	else if (fabs(left - right) < epsilone)
	{
		return 0;
	}
	else if (left - right < 0)
	{
		return -1;
	}
}
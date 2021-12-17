#pragma once
#include<iostream>
#include"Colours.h"
enum EXTREMUM { MAX, MIN };

template <typename TKey>
class Comparator
{
protected:
	enum EXTREMUM extremum_status;
public:
	virtual int compare(const TKey&, const TKey&) const = 0;
	virtual ~Comparator() = default;
};

class ComparatorInt final : public Comparator<int>
{
public:
	ComparatorInt(const enum EXTREMUM status) { this->extremum_status = status; };
	int compare(const int& left, const int& right) const override;
	//enum EXTREMUM extremum_status;
};

class ComparatorStr final : public Comparator<std::string>
{
public:
	ComparatorStr(const enum EXTREMUM status) { this->extremum_status = status; };
	int compare(const std::string& left, const std::string& right) const override; // по длине строки.
};

class ComparatorDouble final : public Comparator<double> //на всякий случай.
{
public:
	ComparatorDouble(const enum EXTREMUM status) { this->extremum_status = status; };
	int compare(const double& left, const double& right) const override; // по длине строки.
};

int ComparatorInt::compare(const int& left, const int& right) const
{
	if (left > right)
	{
		if (this->extremum_status == MAX)
		{
			return 1;
		}
		else if (this->extremum_status == MIN)
		{
			return -1;
		}
	}
	else if (left == right)
	{
		return 0;
	}

	if (this->extremum_status == MAX)
	{
		return -1;
	}
	else if (this->extremum_status == MIN)
	{
		return 1;
	}
}

int ComparatorStr::compare(const std::string& left, const std::string& right) const
{
	if (left.length() > right.length())
	{
		if (this->extremum_status == MAX)
		{
			return 1;
		}
		else if (this->extremum_status == MIN)
		{
			return -1;
		}
	}
	else if (left.length() == right.length())
	{
		return 0;
	}

	if (this->extremum_status == MAX)
	{
		return -1;
	}
	else if (this->extremum_status == MIN)
	{
		return 1;
	}
}

int ComparatorDouble::compare(const double& left, const double& right) const
{
	double epsilone = 0.0000001;
	if (left - right > epsilone)
	{
		if (this->extremum_status == MAX)
		{
			return 1;
		}
		else if (this->extremum_status == MIN)
		{
			return -1;
		}
	}
	else if (fabs(left - right) < epsilone)
	{
		return 0;
	}
	else if (left - right < 0)
	{
		if (this->extremum_status == MAX)
		{
			return -1;
		}
		else if (this->extremum_status == MIN)
		{
			return 1;
		}
	}
}
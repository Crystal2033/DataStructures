#pragma once
#include <iostream>
#include "Colours.h"
#include <fstream>
#include <map>
#include <string>
#include<regex>

class MyException final : public std::exception
{
private:
	//const char* err_msg; \\problem with locale h
	std::string error_msg_str;

public:
	MyException(const std::string& err)
	{
		error_msg_str = err;
	}
	const std::string what_str() const
	{
		return error_msg_str;
	}

};

class Person
{
	std::string name;
	std::pair<Person*, Person*> parents; //mother and father
	std::vector<Person*> children;

public:
	Person(const std::string name)
	{
		this->name = name;
		parents.first = nullptr;
		parents.second = nullptr;
	}

	Person()
	{
		this->name = "";
		parents.first = nullptr;
		parents.second = nullptr;
	}

	std::pair<Person*, Person*> get_parents() const
	{
		return parents;
	}

	void set_name(const std::string& name)
	{
		this->name = name;
	}
	std::string get_name() const
	{
		return name;
	}

	void set_parent(Person* parent)
	{
		if (parents.first == nullptr)
		{
			parents.first = parent;
		}
		else if (parents.second == nullptr)
		{
			if (parents.first->name == parent->name)
			{
				return;
			}
			parents.second = parent;
		}
		else
		{
			throw MyException("Can`t be more than two parents.");
		}

	}

	void set_child(Person* child)
	{
		this->children.push_back(child);
	}

	std::vector<Person*> get_children() const
	{
		return children;
	}

};

std::string check_degree_parents_and_grandparents(const std::pair<std::pair<Person*, Person*>, std::pair<std::pair<Person*, Person*>, std::pair<Person*, Person*>>>& pars_and_grandpars, Person* const& person);
std::vector<std::vector<Person*>> get_grandsons(Person* const& curr_person);
std::pair<std::pair<Person*, Person*>, std::pair<std::pair<Person*, Person*>, std::pair<Person*, Person*>>> find_parents_and_grand_parents(Person* const& curr_person);
void print_parents_and_grands(const std::pair<std::pair<Person*, Person*>, std::pair<std::pair<Person*, Person*>, std::pair<Person*, Person*>>>& ancestors);
std::map<std::string, Person*>::iterator find_person_by_name(const std::map<std::string, Person*>& gene_tree);
std::vector<Person*> get_broths_and_sists(Person* const& curr_person, bool flag_if_binary_broths);
std::vector<std::vector<Person*>> get_cousins(Person* const& curr_person);
void print_names_vector(const std::vector<Person*>& vector);
void print_names_matrix(std::vector<std::vector<Person*>> matrix);
std::vector<Person*> find_multi_family(std::vector<std::string> names_vector, const std::map<std::string, Person*>& gene_tree);
std::string get_relation_degree(const std::map<std::string, Person*>::iterator& first, const std::map<std::string, Person*>::iterator& second, const std::map<std::string, Person*>& gene_tree);
std::string check_degree_parents_and_grandparents(const std::pair<std::pair<Person*, Person*>, std::pair<std::pair<Person*, Person*>, std::pair<Person*, Person*>>>& pars_and_grandpars, Person* const& person);

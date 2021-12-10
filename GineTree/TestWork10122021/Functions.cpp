#pragma once
#include "Functions.h"
std::vector<std::vector<Person*>> get_grandsons(Person* const& curr_person)
{
	std::vector<std::vector<Person*>> grandchildren;
	std::vector<Person*> curr_person_children = curr_person->get_children();
	for (auto curr_child : curr_person_children)
	{
		grandchildren.push_back(curr_child->get_children());
	}
	return grandchildren;
}

std::pair<std::pair<Person*, Person*>, std::pair<std::pair<Person*, Person*>, std::pair<Person*, Person*>>> find_parents_and_grand_parents(Person* const& curr_person)
{
	std::pair<std::pair<Person*, Person*>, std::pair<std::pair<Person*, Person*>, std::pair<Person*, Person*>>> ancestors;
	std::pair<Person*, Person*> curr_person_parents = curr_person->get_parents();
	ancestors.first.first = curr_person_parents.first;
	ancestors.first.second = curr_person_parents.second;
	//перва€ €чейка - родители, втора€ €чейка - родители родитилей на соответствующих местах относительно первой €чейки.
	if (curr_person_parents.first == nullptr)
	{
		ancestors.second.first.first = nullptr;
		ancestors.second.first.second = nullptr;

		ancestors.second.second.first = nullptr;
		ancestors.second.second.second = nullptr;
	}
	else if (curr_person_parents.first != nullptr && curr_person_parents.second == nullptr) //one parent
	{

		std::pair<Person*, Person*> first_parent_parents = curr_person_parents.first->get_parents();
		ancestors.second.first.first = first_parent_parents.first;
		ancestors.second.first.second = first_parent_parents.second;

		ancestors.second.second.first = nullptr;
		ancestors.second.second.second = nullptr;
	}
	else //two parents
	{
		std::pair<Person*, Person*> first_parent_parents = curr_person_parents.first->get_parents();
		std::pair<Person*, Person*> second_parent_parents = curr_person_parents.second->get_parents();

		ancestors.second.first.first = first_parent_parents.first;
		ancestors.second.first.second = first_parent_parents.second;

		ancestors.second.second.first = second_parent_parents.first;
		ancestors.second.second.second = second_parent_parents.second;
	}
	return ancestors;
}


void print_parents_and_grands(const std::pair<std::pair<Person*, Person*>, std::pair<std::pair<Person*, Person*>, std::pair<Person*, Person*>>>& ancestors)
{
	std::cout << azure << "Parents:" << yellow << std::endl;
	if (ancestors.first.first != nullptr)
	{
		std::cout << "1. " << ancestors.first.first->get_name() << std::endl;
	}
	if (ancestors.first.second != nullptr)
	{
		std::cout << "2. " << ancestors.first.second->get_name() << std::endl;
	}
	std::cout << std::endl << azure << "Grandparents:" << yellow << std::endl;


	if (ancestors.second.first.first != nullptr)
	{
		std::cout << ancestors.second.first.first->get_name() << std::endl;
	}
	if (ancestors.second.first.second != nullptr)
	{
		std::cout << ancestors.second.first.second->get_name() << std::endl;
	}

	if (ancestors.second.second.first != nullptr)
	{
		std::cout << yellow << ancestors.second.second.first->get_name() << std::endl;
	}
	if (ancestors.second.second.second != nullptr)
	{
		std::cout << ancestors.second.second.second->get_name() << std::endl;
	}
	std::cout << white << std::endl;
}

std::map<std::string, Person*>::iterator find_person_by_name(const std::map<std::string, Person*>& gene_tree)
{
	std::string curr_person_name;
	std::map<std::string, Person*>::iterator person_iterator;
	while (true)
	{
		std::getline(std::cin, curr_person_name);
		person_iterator = const_cast<std::map<std::string, Person*>&>(gene_tree).find(curr_person_name);
		if (person_iterator == gene_tree.end())
		{
			std::cout << red << "Person wasn`t found. Please, try again." << white << std::endl;
		}
		else
		{
			return person_iterator;
		}
	}

}

std::vector<Person*> get_broths_and_sists(Person* const& curr_person, bool flag_if_binary_broths)
{
	std::vector<Person*> brothers_and_sisters;
	if (curr_person->get_parents().first == nullptr)
	{
		if (!flag_if_binary_broths)
		{
			throw MyException("Current person has not parents. (Tree don`t know about them).");
		}
		else return brothers_and_sisters;
	}
	for (auto br_and_sis : curr_person->get_parents().first->get_children())
	{
		if (br_and_sis->get_name() != curr_person->get_name())
		{
			brothers_and_sisters.push_back(br_and_sis);
		}
	}
	if (curr_person->get_parents().second == nullptr)
	{
		return brothers_and_sisters;
	}
	else
	{
		for (auto br_and_sis : curr_person->get_parents().second->get_children())
		{
			if (br_and_sis->get_name() != curr_person->get_name())
			{
				brothers_and_sisters.push_back(br_and_sis);
			}
		}
	}
	return brothers_and_sisters;
}

std::vector<std::vector<Person*>> get_cousins(Person* const& curr_person)
{
	std::vector<Person*> bin_brothers_and_sisters;
	if (curr_person->get_parents().first == nullptr)
	{
		throw MyException("Current person has not parents. (Tree don`t know about them).");
	}
	std::vector<Person*> first_parent_brothers = get_broths_and_sists(curr_person->get_parents().first, true);
	std::vector<std::vector<Person*>> binary_broths_from_parents;
	if (first_parent_brothers.size() != 0)
	{
		for (auto parent_brother : first_parent_brothers)
		{
			binary_broths_from_parents.push_back(parent_brother->get_children());
		}
	}
	if (curr_person->get_parents().second == nullptr)
	{
		return binary_broths_from_parents;
	}


	std::vector<Person*> second_parent_brothers = get_broths_and_sists(curr_person->get_parents().second, true);
	if (second_parent_brothers.size() != 0)
	{
		for (auto parent_brother : second_parent_brothers)
		{
			binary_broths_from_parents.push_back(parent_brother->get_children());
		}
	}
	return binary_broths_from_parents;
}


void print_names_vector(const std::vector<Person*>& vector)
{
	for (auto it : vector)
	{
		std::cout << yellow << it->get_name() << std::endl;
	}
	std::cout << white << std::endl;
}

void print_names_matrix(std::vector<std::vector<Person*>> matrix)
{
	for (auto strings : matrix)
	{
		for (auto cells : strings)
		{
			std::cout << yellow << cells->get_name() << std::endl;
		}
	}
	std::cout << white << std::endl;
}

std::vector<Person*> find_multi_family(std::vector<std::string> names_vector, const std::map<std::string, Person*>& gene_tree)
{
	std::vector<Person*> multi_family_parents;
	for (auto person : names_vector)
	{
		auto iterator = gene_tree.find(person);
		if ((iterator->second->get_children().size()) >= 3)
		{
			multi_family_parents.push_back(iterator->second);
		}
	}
	return multi_family_parents;
}

std::string set_degree_vector_case(const std::string& check_on_degree, std::vector<Person*> vector, Person* const& second_person)
{
	std::string degree;
	for (auto child : vector)
	{
		if (child->get_name() == second_person->get_name())
		{
			degree = check_on_degree;
		}
	}
	return degree;
}

std::string set_degree_matrix_case(const std::string& check_on_degree, std::vector<std::vector<Person*>> matrix, Person* const& second_person)
{
	std::string degree;
	for (auto strings : matrix)
	{
		for (auto it : strings)
		{
			if (it->get_name() == second_person->get_name())
			{
				degree = check_on_degree;
			}
		}
	}
	return degree;
}

std::string get_relation_degree(const std::map<std::string, Person*>::iterator& first_person, const std::map<std::string, Person*>::iterator& second_person, const std::map<std::string, Person*>& gene_tree)
{
	std::string degree;
	std::pair<std::pair<Person*, Person*>, std::pair<std::pair<Person*, Person*>, std::pair<Person*, Person*>>> parents_and_grand_parents = find_parents_and_grand_parents(first_person->second);
	degree = check_degree_parents_and_grandparents(parents_and_grand_parents, second_person->second);
	if (degree.length() != 0)
	{
		return degree;
	}
	degree = set_degree_vector_case("child", first_person->second->get_children(), second_person->second);
	if (degree.length() != 0)
	{
		return degree;
	}

	std::vector<std::vector<Person*>> grand_children = get_grandsons(first_person->second);
	degree = set_degree_matrix_case("grand child", grand_children, second_person->second);
	if (degree.length() != 0)
	{
		return degree;
	}

	std::vector<Person*> brothers_and_sisters = get_broths_and_sists(first_person->second, false);
	degree = set_degree_vector_case("brother(sister)", brothers_and_sisters, second_person->second);
	if (degree.length() != 0)
	{
		return degree;
	}

	std::vector<std::vector<Person*>> bin_brothers_and_sisters = get_cousins(first_person->second);
	degree = set_degree_matrix_case("cousin", bin_brothers_and_sisters, second_person->second);
	if (degree.length() != 0)
	{
		return degree;
	}

	std::vector<Person*> parents_brothers_and_sisters;
	if (first_person->second->get_parents().first != nullptr)
	{
		parents_brothers_and_sisters = get_broths_and_sists(first_person->second->get_parents().first, false);
		degree = set_degree_vector_case("uncle", parents_brothers_and_sisters, second_person->second);
		if (degree.length() != 0)
		{
			return degree;
		}
	}

	if (first_person->second->get_parents().second != nullptr)
	{
		parents_brothers_and_sisters = get_broths_and_sists(first_person->second->get_parents().second, false);
		degree = set_degree_vector_case("uncle", parents_brothers_and_sisters, second_person->second);
		if (degree.length() != 0)
		{
			return degree;
		}
	}

	return degree;

}

std::string check_degree_parents_and_grandparents(const std::pair<std::pair<Person*, Person*>, std::pair<std::pair<Person*, Person*>, std::pair<Person*, Person*>>>& pars_and_grandpars, Person* const& person)
{
	std::string degree;
	if (pars_and_grandpars.first.first != nullptr)
	{
		if (pars_and_grandpars.first.first->get_name() == person->get_name())
		{
			degree = "parent";
		}
	}
	if (pars_and_grandpars.first.second != nullptr)
	{
		if (pars_and_grandpars.first.second->get_name() == person->get_name())
		{
			degree = "parent";
		}
	}

	if (pars_and_grandpars.second.first.first != nullptr)
	{
		if (pars_and_grandpars.second.first.first->get_name() == person->get_name())
		{
			degree = "grandparent";
		}
	}
	if (pars_and_grandpars.second.first.second != nullptr)
	{
		if (pars_and_grandpars.second.first.second->get_name() == person->get_name())
		{
			degree = "grandparent";
		}
	}

	if (pars_and_grandpars.second.second.first != nullptr)
	{
		if (pars_and_grandpars.second.second.first->get_name() == person->get_name())
		{
			degree = "grandparent";
		}
	}
	if (pars_and_grandpars.second.second.second != nullptr)
	{
		if (pars_and_grandpars.second.second.second->get_name() == person->get_name())
		{
			degree = "grandparent";
		}
	}
	return degree;
}

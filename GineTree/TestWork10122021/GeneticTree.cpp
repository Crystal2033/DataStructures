#include "Functions.h"

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		std::cout << red << "You have forgotten to input file." << white <<std::endl;
		return -1;
	}
	std::ifstream file;
	file.open(argv[1]);
	if (!file.good())
	{
		std::cout << red << "There is something wrong with file openning." << white << std::endl;
		return -1;
	}

	std::string file_str;
	std::regex regular;
	std::cmatch result_of_reg;
	regular = "([\\<])([\\w]+)([\\>])([ ]*)([\\<])([\\w]+)([\\>])";
	std::vector<Person*> persons;
	std::map<std::string, Person*> gene_tree;
	std::map< std::string, Person*>::iterator iterator_parent;
	std::map< std::string, Person*>::iterator iterator_child;
	std::map< std::string, Person*>::iterator person_iterator;

	std::vector<std::string> names_for_clearing; //for deleting persons at the end.
	try
	{
		std::cout << "Was written from file:"<< std::endl;
		while (std::getline(file, file_str))
		{
			Person* parent = new Person;
			Person* child = new Person;
			if (file_str.length() == 0)
			{
				break;
			}
			std::cout << purple << file_str << white << std::endl;
			if (!std::regex_match(file_str.c_str(), result_of_reg, regular))
			{
				std::cout << "Bad file" << std::endl;
				file.close();
				return -1;
			}
			//std::cout << cyan << "Parent: " << white << result_of_reg[2] << cyan << " Child: " << white << result_of_reg[6] << white << std::endl;
			//2 - parent, 6 - child
			std::string parent_name = result_of_reg[2].str();
			std::string child_name = result_of_reg[6].str();

			parent->set_name(parent_name);
			child->set_name(child_name);
			iterator_parent = gene_tree.find(parent_name);
			iterator_child = gene_tree.find(child_name);

			if ((iterator_parent == gene_tree.end()) && (iterator_child == gene_tree.end())) // not found parent and not found child
			{
				child->set_parent(parent);
				parent->set_child(child);
				gene_tree.insert(std::make_pair(parent_name, parent));
				gene_tree.insert(std::make_pair(child_name, child));
				names_for_clearing.push_back(parent_name);
				names_for_clearing.push_back(child_name);

			}
			else if ((iterator_parent != gene_tree.end()) && (iterator_child == gene_tree.end())) //parent exists, child - don`t
			{
				child->set_parent(iterator_parent->second);
				iterator_parent->second->set_child(child);
				gene_tree.insert(std::make_pair(child_name, child));
				names_for_clearing.push_back(child_name);
			}
			else if ((iterator_parent != gene_tree.end()) && (iterator_child != gene_tree.end())) //parent exists, child - don`t
			{
				iterator_child->second->set_parent(iterator_parent->second);
				iterator_parent->second->set_child(iterator_child->second);
				gene_tree.insert(std::make_pair(child_name, child));
				//names_for_clearing.push_back(child_name);
			}
			else // child exists, parent - don`t
			{
				iterator_child->second->set_parent(parent);
				parent->set_child(iterator_child->second);
				gene_tree.insert(std::make_pair(parent_name, parent));
				names_for_clearing.push_back(parent_name);
			}
		}
		std::cout << yellow << "---------------------------------------------------------------------" << white << std::endl;
		//1. Find grandmother/parents/grandfather
		std::string curr_person_name;
		std::cout << cyan << "Input name of person. Programm will find his parents and grandparents." << white << std::endl;
		person_iterator = find_person_by_name(gene_tree);

		std::pair<std::pair<Person*, Person*>, std::pair<std::pair<Person*, Person*>, std::pair<Person*, Person*>>> ancestors = find_parents_and_grand_parents(person_iterator->second);
		print_parents_and_grands(ancestors);
		std::cout << yellow << "---------------------------------------------------------------------" << white << std::endl;
		//2. Find children, grandchildren
		std::cout << cyan << "Input name of person. Programm will find his children and grandchildren." << white << std::endl;
		person_iterator = find_person_by_name(gene_tree);

		std::vector<Person*> curr_person_children = person_iterator->second->get_children();
		std::cout << azure << "Children:" << yellow << std::endl;
		print_names_vector(curr_person_children);
	
		std::vector<std::vector<Person*>> grand_children = get_grandsons(person_iterator->second);
		std::cout << azure << "Grand children:" << yellow << std::endl;
		print_names_matrix(grand_children);
		std::cout << yellow << "---------------------------------------------------------------------" << white << std::endl;
		//3. Find brothers/sisters and binary-brothers/sisters
		std::cout << cyan << "Input name of person. Programm will find his brothers/sisters and binary brothers/sisters." << white << std::endl;
		person_iterator = find_person_by_name(gene_tree);
		std::vector<Person*> brothers_and_sisters;
		brothers_and_sisters = get_broths_and_sists(person_iterator->second, false);
		std::cout << azure << "Brothers and sisters: " << std::endl;
		print_names_vector(brothers_and_sisters);

		std::vector<std::vector<Person*>> binary_brothers = get_cousins(person_iterator->second);
		std::cout << azure << "Binary brothers and sisters: " << std::endl;
		print_names_matrix(binary_brothers);
		std::cout << yellow << "---------------------------------------------------------------------" << white << std::endl;
		//4. Find a degree of relation-ship between two persons.
		std::cout << cyan << "Finding degree of relationship in family between two persons." << std::endl;
		std::map< std::string, Person*>::iterator second_person_iterator;
		while (true)
		{
			std::cout << cyan << "Input first name:" << white << std::endl;
			person_iterator = find_person_by_name(gene_tree);
			std::cout << cyan << "Input second name:" << white << std::endl;
			second_person_iterator = find_person_by_name(gene_tree);
			if (person_iterator->second->get_name() != second_person_iterator->second->get_name())
			{
				break;
			}
			std::cout << red << "Both names are the same, try again." << white << std::endl;
		}

		std::string relation_degree = get_relation_degree(person_iterator, second_person_iterator, gene_tree);
		if (relation_degree.size() != 0)
		{
			std::cout << azure << second_person_iterator->second->get_name() << cyan << " is " << yellow << relation_degree << cyan << " for " << azure << person_iterator->second->get_name() << white << std::endl << std::endl;
		}
		else
		{
			std::cout << green << "There is low version of program to search deepper in gine tree. Will develop in future. Sorry for inconveniences." << white << std::endl;
		}
		std::cout << yellow << "---------------------------------------------------------------------" << white << std::endl;
		//5. Find all multi-family
		std::cout << cyan << "Multi families:" << white << std::endl;
		std::vector<Person*> multi_family_parents = find_multi_family(names_for_clearing, gene_tree);
		std::cout << azure << "Multi family parents ( >=3 kids): " << std::endl;
		print_names_vector(multi_family_parents);
		std::cout << yellow << "---------------------------------------------------------------------" << white << std::endl;

	}
	catch (MyException& err)
	{
		std::cout << red << err.what_str() << white <<std::endl;
	}

	
	auto clean_iterator = names_for_clearing.begin();
	while (clean_iterator != names_for_clearing.end())
	{
		person_iterator = gene_tree.find(*clean_iterator);
		delete person_iterator->second;
		clean_iterator++;
	}
	file.close();

	return 0;
}
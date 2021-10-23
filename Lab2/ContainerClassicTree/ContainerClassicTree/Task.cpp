#include <iostream>
#include "Colours.h"
#include "Containers.h"
#include "Comparator.h"

int main(void)
{
	Comparator<int>* comparator = new ComparatorInt;
	ClassicBinaryTree<int, std::string> tree(comparator);
	//AVL_Tree<int, std::string> avl_tree(comparator);
	try
	{
		tree.add(10, "MyCar1");
		tree.add(5, "MyCar2");
		tree.add(3, "MyCar3");
		tree.add(12, "MyCar4");
		tree.add(2, "MyCar5");
		tree.prefix_stepover_tree(print_tree);


		/*avl_tree.add(10, "Mazzeratti");
		avl_tree.add(15, "Lada");
		avl_tree.add(16, "BMW");
		avl_tree.infix_stepover_tree(print_tree);*/
		/*tree.remove(10);
		std::cout << blue << tree.get_data(10) << white << std::endl;*/
	}
	catch (ClassicBinaryTree<int, std::string>::FindNodeError& err)
	{
		std::cout << red << err.what() << white << std::endl;
	}
	catch (ClassicBinaryTree<int, std::string>::KeyExistsError& err)
	{
		std::cout << red << err.what() << white << std::endl;
	}


	return 0;
}
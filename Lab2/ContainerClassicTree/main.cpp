#include <iostream>
#include "Colours.h"

#include "comparer.h"
#include "BST.h"
#include "AVL.h"
#include "RB.h"
#include "Splay.h"
#include <string>

int main(int argc, char* argv[])
{
	
	comparer<int>* comparator = new ComparatorInt;
	ClassicBinaryTree<int, std::string> tree(comparator);
	associative_container<int, std::string>* my_tree = new RB_Tree<int, std::string>(comparator);
	AVL_Tree<int, std::string> avl_tree(comparator);
	ClassicBinaryTree<int, std::string> classic_tree(comparator);
	RB_Tree<int, std::string> rb_tree(comparator);
	Splay_Tree<int, std::string> splay_tree(comparator);
	try
	{
		std::cout << cyan <<  "Work was completed by Kulikov Paul. Group M80-211B-20" << white << std::endl;
	}
	catch (ClassicBinaryTree<int, std::string>::FindNodeError& err)
	{
		std::cout << red << err.what() << " Wrong key: " << err.get_key() << white << std::endl;
	}
	catch (ClassicBinaryTree<int, std::string>::KeyExistsError& err)
	{
		std::cout << red << err.what() << " Existing key: " << err.get_key() << white << std::endl;
	}

	return 0;
}
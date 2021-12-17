#include"Comparator.h"
#include "BinaryHeap.h"
#include "BinomialHeap.h"
#include "FibonacciHeap.h"


int main(int argc, char* argv[])
{
	try
	{
		//Comparator<int>* comparator = new ComparatorInt;
		//BinaryHeap<int, std::string>* binary_heap;
		//BinaryHeap<int, std::string>* second_binary_heap;
		//MergeableHeap<int, std::string>* heap_ptr = binary_heap->create_object(comparator);
		//MergeableHeap<int, std::string>* second_heap_ptr = second_binary_heap->create_object(comparator);
		//for (int i = 0; i < 10; i++)
		//{
		//	heap_ptr->insert(i, std::to_string(i));
		//}
		//std::cout << blue << "Heap_ptr:" << white << std::endl;
		//heap_ptr->print_heap();
		//for (int i = 10; i < 20; i++)
		//{
		//	second_heap_ptr->insert(i, std::to_string(i));
		//}
		//std::cout << blue << "second_heap_ptr:" << white << std::endl;
		//second_heap_ptr->print_heap();


		//heap_ptr->merge(second_heap_ptr);
		//heap_ptr->print_heap();
		//for (int i = 0; i < 19; i++)
		//{
		//	heap_ptr->remove();
		//	std::cout << blue << "Current extremum is: " << cyan << heap_ptr->get_data() << white << std::endl;
		//}
		//heap_ptr->print_heap();
		

		//std::cout << blue << "after =:" << white << std::endl;
		//second_heap_ptr->~MergeableHeap();
		//binary_heap->print_heap();
		//second_binary_heap->print_heap();


		/*Comparator<int>* comparator = new ComparatorInt;
		BinomialHeap<int, std::string>* binom_heap;
		BinomialHeap<int, std::string>* second_binom_heap;
		MergeableHeap<int, std::string>* heap_ptr = binom_heap->create_object(comparator);
		MergeableHeap<int, std::string>* second_heap_ptr = second_binom_heap->create_object(comparator);

		
		for (int i = 0; i < 15; i++)
		{
			heap_ptr->insert(i, std::to_string(i));
		}

		for (int i = 15; i < 120; i++)
		{
			second_heap_ptr->insert(i, std::to_string(i));
		}
		std::cout << blue <<  "HEAP FIRST:" << white <<  std::endl;
		heap_ptr->print_heap();
		std::cout << yellow  << "The extremum is: " << green << heap_ptr->get_data() << white << std::endl;


		std::cout << blue << "HEAP SECOND:" << white << std::endl;
		second_heap_ptr->print_heap();
		std::cout << yellow << "The extremum is: " << green << second_heap_ptr->get_data() << white << std::endl;

		heap_ptr->merge(second_heap_ptr);
		std::cout << std::endl << blue << "MERGED:" << white << std::endl;
		heap_ptr->print_heap();
		std::cout << yellow << "The extremum is: " << green << heap_ptr->get_data() << white << std::endl;
		for (int i = 0; i < 119; i++)
		{
			heap_ptr->remove();
		}

		std::cout << std::endl << blue << "AFTER DELETE:" << white << std::endl;
		heap_ptr->print_heap();
		std::cout << yellow << "The extremum is: " << green << heap_ptr->get_data() << white << std::endl;*/
		

		Comparator<int>* comparator = new ComparatorInt(MAX);
		FibonacciHeap<int, std::string>* fibon_heap;
		FibonacciHeap<int, std::string>* second_fibon_heap;
		MergeableHeap<int, std::string>* heap_ptr = fibon_heap->create_object(comparator);
		MergeableHeap<int, std::string>* second_heap_ptr = second_fibon_heap->create_object(comparator);

		for (int i = 6; i > -1; i--)
		{
			heap_ptr->insert(i, std::to_string(i));
		}
		
		heap_ptr->print_heap();
		heap_ptr->remove();
		std::cout << green << "AFTER delete: " << white << std::endl;
		heap_ptr->print_heap();
		heap_ptr->remove();
		std::cout << green << "AFTER delete: " << white << std::endl;
		heap_ptr->print_heap();
		heap_ptr->remove();
		std::cout << green << "AFTER delete: " << white << std::endl;
		heap_ptr->print_heap();
		heap_ptr->remove();
		std::cout << green << "AFTER delete: " << white << std::endl;
		heap_ptr->print_heap();

		heap_ptr->remove();
		std::cout << green << "AFTER delete: " << white << std::endl;
		heap_ptr->print_heap();
		std::cout << blue << "Current extremum is: " << cyan << heap_ptr->get_data() << white << std::endl;
		for (int i = 2; i < 30; i++)
		{
			heap_ptr->insert(i, std::to_string(i));
		}
		heap_ptr->print_heap();
		std::cout << blue << "Current extremum is: " << cyan << heap_ptr->get_data() << white << std::endl;

		for (int i = 29; i > 0; i--)
		{
			heap_ptr->remove();
			std::cout << blue << "Current extremum is: " << cyan << heap_ptr->get_data() << white << std::endl;
		}
		delete comparator;
	}
	catch (AddExists<int>& err)
	{
		std::cout << red << err.what() << " Error key: " << yellow << err.get_key() << white << std::endl;
	}
	catch (GetEmpty<int>& err)
	{
		std::cout << red << err.what() << white << std::endl;
	}
	catch (RemEmpty<int>& err)
	{
		std::cout << red << err.what() << white << std::endl;
	}
	catch (MergeSameKeys<int>& err)
	{
		std::cout << red << err.what() << " Error key: " << yellow << err.get_key() << white << std::endl;
	}
	catch (...)
	{
		std::cout << red << "Unknown error." << white << std::endl;
	}
	return 0;

}
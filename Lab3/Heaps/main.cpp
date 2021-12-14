#include"Comparator.h"
#include "BinaryHeap.h"
#include "FibonacciHeap.h"


int main(int argc, char* argv[])
{
	//std::cout << green << "Hello world!" << white << std::endl;
	try
	{
		Comparator<int>* comparator = new ComparatorInt;
		BinaryHeap<int, std::string>* bin_heap;
		BinaryHeap<int, std::string>* bin_heap_second;
		MergeableHeap<int, std::string>* heap_ptr_second = bin_heap_second->create_object(comparator);
		
		MergeableHeap<int, std::string>* heap_ptr = bin_heap->create_object(comparator);
		bin_heap = reinterpret_cast<BinaryHeap<int, std::string>*>(heap_ptr);

		for (int i = -10; i < 25; i++)
		{
			heap_ptr->insert(i, std::to_string(double(i)));
		}
		std::cout << cyan << "First heap:" << white << std::endl;
		heap_ptr->print_heap();

		BinaryHeap<int, std::string>* bin_heap_third = bin_heap;
		std::cout << cyan << "====:" << white << std::endl;
		bin_heap_third->print_heap();

		BinaryHeap<int, std::string>* bin_heap_fourth(bin_heap_third);
		std::cout << cyan << "Copy:" << white << std::endl;
		bin_heap_fourth->print_heap();

		for (int i = 25; i < 50; i++)
		{
			heap_ptr_second->insert(i, std::to_string(double(i)));
		}
		std::cout << cyan << "Second heap:" << white << std::endl;
		heap_ptr_second->print_heap();

		heap_ptr_second->merge(heap_ptr);
		std::cout << azure << "After merger:" << white << std::endl;
		std::cout << cyan << "First heap:" << white << std::endl;
		heap_ptr->print_heap();
		std::cout << cyan << "Second heap:" << white << std::endl;
		heap_ptr_second->print_heap();
		/*for (int i = 19; i > -5; i--)
		{
			heap_ptr->remove();
			std::cout << yellow << "Remove[" << green << i << yellow <<  "]: " << white << std::endl;
			std::cout << "Data from heap: " << green << heap_ptr->get_data() << white << std::endl;
			heap_ptr->print_heap();
			std::cout << yellow << "-------------------------------------------------------" << white << std::endl;
		}*/
		
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
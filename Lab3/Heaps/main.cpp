#include"Comparator.h"
#include "BinaryHeap.h"
#include "FibonacciHeap.h"
#include "BinomialHeap.h"


int main(int argc, char* argv[])
{
	//std::cout << green << "Hello world!" << white << std::endl;
	try
	{
		Comparator<int>* comparator = new ComparatorInt;
		BinomialHeap<int, std::string>* binom_heap;
		MergeableHeap<int, std::string>* heap_ptr = binom_heap->create_object(comparator);
		/*heap_ptr->insert(10, "10");
		heap_ptr->insert(8, "8");
		heap_ptr->insert(20, "20");
		heap_ptr->insert(30, "30");
		heap_ptr->insert(40, "40");
		heap_ptr->insert(25, "25");
		heap_ptr->insert(15, "15");
		heap_ptr->insert(35, "35");
		heap_ptr->insert(0, "0");*/
		for (int i = 31; i > 0; i--)
		{
			heap_ptr->insert(i, std::to_string(i));
		}
		heap_ptr->print_heap();
		std::cout << yellow  << "The extremum is: " << purple << heap_ptr->get_data() << white << std::endl;

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
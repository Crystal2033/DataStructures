#pragma once
#include"AbstractHeapClass.h"
#include<list>
#include<map>
#include "Exceptions.h"

template <typename TKey, typename TValue>
class FibonacciHeap : public MergeableHeap<TKey, TValue>
{
private:
	class HeapNode
	{
	public:
		TKey key;
		TValue value;
		int degree;
		HeapNode(const TKey& key, const TValue& value);
		HeapNode() = default;
		std::list<HeapNode*>* children_ptr;
	};
	Comparator<TKey>* comparator;
	HeapNode* extremum_root;
	std::map<TKey, TKey> checker_map;
	std::list<HeapNode*> root_list;
	//class Clock near abstract heap class
	FibonacciHeap(Comparator<TKey>* comparator);
	
public:
	static MergeableHeap<TKey, TValue>* create_object(Comparator<TKey>* comparator);

	void merge(const MergeableHeap<TKey, TValue>* heap) override;
	void insert(const TKey&, const TValue&) override;
	std::string who_am_i() const override;
	TValue get_data() const override;
	void print_heap() const override;
	void remove() override;

	~FibonacciHeap();
private:
	void find_new_extremum_in_list(std::list<HeapNode*> const& list);
	void check_same_keys(const FibonacciHeap<TKey, TValue>* binom_second_heap);
	void print_child(std::list<HeapNode*>* children_ptr, int counter) const;
	void remove_root_list();
	void remove_child(std::list<HeapNode*>* children_ptr);
	void vector_resize(int degree, std::vector<HeapNode*>& vector) const;
	void copy_list_to_root_list(std::list<HeapNode*>* const& list_for_copy);
 
};

#pragma region NodeImplementation
template <typename TKey, typename TValue>
FibonacciHeap<TKey, TValue>::HeapNode::HeapNode(const TKey& key, const TValue& value)
{
	degree = 0;
	this->key = key;
	this->value = value;
	this->children_ptr = new std::list<HeapNode*>();
}
#pragma endregion


#pragma region TestFunction
template <typename TKey, typename TValue>
std::string FibonacciHeap<TKey, TValue>::who_am_i() const
{
	return "Fibonacci Heap.";
}

template <typename TKey, typename TValue>
void FibonacciHeap<TKey, TValue>::print_heap() const
{
	if (root_list.empty())
	{
		std::cout << red << "PrintHeap: Your heap is empty" << white << std::endl;
	}
	int counter = 0;
	for (auto iterator = root_list.begin(); iterator != root_list.end(); iterator++)
	{
		std::cout << cyan << "Level: " << green << counter << yellow << " [" << (*iterator)->key << " : " << (*iterator)->value << "]," << cyan << " degree =  " << blue << (*iterator)->degree << white << std::endl;
		print_child((*iterator)->children_ptr, counter);
	}
}

template <typename TKey, typename TValue>
void FibonacciHeap<TKey, TValue>::print_child(std::list<HeapNode*>* children_ptr, int counter) const
{
	counter++;
	for (auto iterator = children_ptr->begin(); iterator != children_ptr->end(); iterator++)
	{
		if (!(*iterator)->children_ptr->empty())
		{
			print_child((*iterator)->children_ptr, counter);
		}
		std::cout << cyan << "Level: " << green << counter << yellow << " [" << (*iterator)->key << " : " << (*iterator)->value << "]," << cyan << " degree =  " << blue << (*iterator)->degree << white << std::endl;
	}

}
#pragma endregion

#pragma region SameKeysChecker
template <typename TKey, typename TValue>
void FibonacciHeap<TKey, TValue>::check_same_keys(const FibonacciHeap<TKey, TValue>* binom_second_heap)
{
	bool second_size_equ_or_less;
	if (binom_second_heap->checker_map.size() <= checker_map.size())
	{
		second_size_equ_or_less = true;
	}
	else
	{
		second_size_equ_or_less = false;
	}
	auto iterator = (second_size_equ_or_less == true) ? binom_second_heap->checker_map.begin() : checker_map.begin();

	for (; iterator != ((second_size_equ_or_less == true) ? binom_second_heap->checker_map.end() : checker_map.end()); iterator++)
	{
		if (second_size_equ_or_less == true)
		{
			auto check_iterator = checker_map.find((*iterator).first);
			if (check_iterator != checker_map.end())
			{
				throw MergeSameKeys<TKey>("Impossible to merge two heaps. Because there are two same keys.", (*iterator).first);
			}
		}
		else
		{
			auto check_iterator = binom_second_heap->checker_map.find((*iterator).first);
			if (check_iterator != binom_second_heap->checker_map.end())
			{
				throw MergeSameKeys<TKey>("Impossible to merge two heaps. Because there are two same keys.", (*iterator).first);
			}
		}
	}
}
#pragma endregion



#pragma region Constructor and Destructor
template <typename TKey, typename TValue>
FibonacciHeap<TKey, TValue>::FibonacciHeap(Comparator<TKey>* comparator)
{
	this->comparator = comparator;
	extremum_root = nullptr;
}

template <typename TKey, typename TValue>
FibonacciHeap<TKey, TValue>::~FibonacciHeap()
{
	remove_root_list();
}

template <typename TKey, typename TValue>
void FibonacciHeap<TKey, TValue>::remove_root_list()
{
	for (auto iterator = root_list.begin(); iterator != root_list.end(); iterator++)
	{
		remove_child((*iterator)->children_ptr);
		std::cout << red << "Deleting... " << blue << (*iterator)->key << white << std::endl;
		delete (*iterator);
	}
}


template <typename TKey, typename TValue>
void FibonacciHeap<TKey, TValue>::remove_child(std::list<HeapNode*>* children_ptr)
{
	for (auto iterator = children_ptr->begin(); iterator != children_ptr->end(); iterator++)
	{
		if (!(*iterator)->children_ptr->empty())
		{
			remove_child((*iterator)->children_ptr);
		}
		std::cout << red << "Deleting... " << blue << (*iterator)->key << white << std::endl;
		delete (*iterator);
	}
}
#pragma endregion

#pragma region Factory
template <typename TKey, typename TValue>
MergeableHeap<TKey, TValue>* FibonacciHeap<TKey, TValue>::create_object(Comparator<TKey>* comparator)
{
	//return nullptr;
	return new FibonacciHeap(comparator);
}
#pragma endregion

#pragma region Insertion
template <typename TKey, typename TValue>
void FibonacciHeap<TKey, TValue>::insert(const TKey& key, const TValue& value)
{
	auto iterator = this->checker_map.find(key);
	if (iterator != this->checker_map.end())
	{
		throw AddExists<TKey>("Inserting error. Key already exists.", key);
	}

	HeapNode* heap_node = new HeapNode(key, value);
	root_list.push_back(heap_node);
	if (root_list.size() == 1)
	{
		extremum_root = heap_node;
		checker_map.insert(std::make_pair(key, key));
		return;
	}

	if (comparator->compare(heap_node->key, extremum_root->key) > 0)
	{
		extremum_root = heap_node;
	}
}
#pragma endregion

#pragma region Getting data
template <typename TKey, typename TValue>
TValue FibonacciHeap<TKey, TValue>::get_data() const
{
	if (root_list.empty())
	{
		throw GetEmpty<TKey>("Get data error. Your Fibonacci heap is empty.");
	}
	return extremum_root->value;
}
#pragma endregion

#pragma region Removing
template <typename TKey, typename TValue>
void FibonacciHeap<TKey, TValue>::find_new_extremum_in_list(std::list<HeapNode*> const& list)
{
	for (auto find_extremum_it = list.begin(); find_extremum_it != list.end(); find_extremum_it++)
	{
		if (find_extremum_it == list.begin())
		{
			extremum_root = *find_extremum_it;
		}

		if (comparator->compare((*find_extremum_it)->key, extremum_root->key) > 0)
		{
			extremum_root = *find_extremum_it;
		}
	}
}

template <typename TKey, typename TValue>
void FibonacciHeap<TKey, TValue>::copy_list_to_root_list(std::list<HeapNode*>* const& list_for_copy)
{
	if (list_for_copy->empty())
	{
		return;
	}
	auto it = list_for_copy->begin();
	for (; it != list_for_copy->end(); it++)
	{
		root_list.push_back((*it));
	}
}

template <typename TKey, typename TValue>
void FibonacciHeap<TKey, TValue>::remove()
{
	if (root_list.empty())
	{
		throw RemEmpty<TKey>("Remove data error. Your Fibonacci heap is empty.");
	}

	copy_list_to_root_list(extremum_root->children_ptr);

	for (auto it = root_list.begin(); it != root_list.end(); it++)
	{
		if ((*it)->key == extremum_root->key)
		{
			checker_map.erase(extremum_root->key);
			extremum_root = nullptr;
			it = root_list.erase(it);
			break;
		}
	}
	find_new_extremum_in_list(root_list);
	
	
	//collecting
	std::vector<HeapNode*> degree_ptr_vector;
	//int vector_size = 0;
	for (auto iterator = root_list.begin(); iterator != root_list.end();)
	{
		//vector_size = degree_ptr_vector.size();
		//std::cout << (*iterator)->degree << " vs " << vector_size - 1 << std::endl;
		//if ((*iterator)->degree > vector_size - 1)
		//{
		//	degree_ptr_vector.resize((*iterator)->degree + 1);
		//}
		vector_resize((*iterator)->degree, degree_ptr_vector);
		HeapNode* cur_iter_node = *iterator;
		if (degree_ptr_vector[cur_iter_node->degree] == nullptr)
		{
			degree_ptr_vector[cur_iter_node->degree] = cur_iter_node;
			iterator++;
			continue;
		}
		else
		{
			while (true)
			{
				HeapNode* vector_same_degree_node = degree_ptr_vector[cur_iter_node->degree];
				if (comparator->compare(vector_same_degree_node->key, cur_iter_node->key) > 0)
				{
					auto iter_finder = root_list.begin();
					for (; iter_finder != root_list.end(); iter_finder++)
					{
						if ((*iter_finder)->key == cur_iter_node->key)
						{
							break;
						}
					}
					if (iterator == iter_finder)
					{
						iterator = root_list.erase(iterator);
					}
					else
					{
						iter_finder = root_list.erase(iter_finder);
					}

					vector_same_degree_node->children_ptr->push_back(cur_iter_node);
					degree_ptr_vector[vector_same_degree_node->degree] = nullptr;
					vector_same_degree_node->degree++;
					vector_resize(vector_same_degree_node->degree, degree_ptr_vector);
					if (degree_ptr_vector[vector_same_degree_node->degree] == nullptr)
					{
						degree_ptr_vector[vector_same_degree_node->degree] = vector_same_degree_node;
						break;
					}
					else
					{
						cur_iter_node = vector_same_degree_node;
						//iterator++;
						continue;
					}
				}
				else
				{
					auto iter_finder = root_list.begin();
					for (; iter_finder != root_list.end(); iter_finder++)
					{
						if ((*iter_finder)->key == vector_same_degree_node->key)
						{
							break;
						}
					}
					iter_finder = root_list.erase(iter_finder);
					cur_iter_node->children_ptr->push_back(vector_same_degree_node);
					degree_ptr_vector[vector_same_degree_node->degree] = nullptr;
					cur_iter_node->degree++;
					vector_resize(cur_iter_node->degree, degree_ptr_vector);

					if (degree_ptr_vector[cur_iter_node->degree] == nullptr)
					{
						degree_ptr_vector[cur_iter_node->degree] = cur_iter_node;
						break;
					}
					else
					{
						continue;
					}
				}
			}
			if (iterator != root_list.end()) //root_list.erase(iterator); AFFECT
			{
				iterator++;
			}
		}
	}
}

template <typename TKey, typename TValue>
void FibonacciHeap<TKey, TValue>::vector_resize(int degree, std::vector<HeapNode*>& vector) const
{
	int vector_size = 0;
	vector_size = vector.size();
	//std::cout << degree << " vs " << vector_size - 1 << std::endl;
	if (degree > vector_size - 1)
	{
		vector.resize(degree + 1, nullptr);
	}
}
#pragma endregion

#pragma region Merge
template <typename TKey, typename TValue>
void FibonacciHeap<TKey, TValue>::merge(const MergeableHeap<TKey, TValue>* heap)
{
	FibonacciHeap<TKey, TValue>* fib_heap = reinterpret_cast<FibonacciHeap<TKey, TValue>*>(const_cast<MergeableHeap<TKey, TValue>*>(heap));
	check_same_keys(fib_heap);
	if (comparator->compare(fib_heap->extremum_root->key, extremum_root->key) > 0)
	{
		extremum_root = fib_heap->extremum_root;
	}
	auto it = root_list.end();
	this->root_list.splice(it, fib_heap->root_list); //O(1)
	fib_heap->root_list = std::list<HeapNode*>();
	fib_heap->extremum_root = nullptr;
}
#pragma endregion

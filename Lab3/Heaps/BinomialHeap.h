#pragma once
#include"AbstractHeapClass.h"
#include "Exceptions.h"
#include <list>
#include <map>
static enum Direction {FRONT, BACK};
template <typename TKey, typename TValue>
class BinomialHeap : public MergeableHeap<TKey, TValue>
{
	class HeapNode
	{
	public:
		TKey key;
		TValue value;
		int degree;
		HeapNode(const TKey& key, const TValue& value);
		HeapNode() = default;
		std::list<HeapNode*>* children_ptr;
		//~HeapNode();
		void set_new_degree(const int degree) { this->degree = degree; }

	};
private:
	Comparator<TKey>* comparator;
	std::list<HeapNode*> root_list;
	HeapNode* extremum_root;
	std::map<TKey, TKey> checker_map;
	void check_same_keys(const BinomialHeap<TKey, TValue>* binom_second_heap);
	void same_degree_concat(HeapNode*& heap_node, HeapNode* const& old_heap_node, std::list<HeapNode*>& list, enum Direction dir);
	void push_back_or_front_concat(std::list<HeapNode*>& list, HeapNode* const& old_heap_node, enum Direction dir);

	void find_new_extremum_in_list(std::list<HeapNode*> const& list);
	BinomialHeap(Comparator<TKey>* comparator);

	void copy_list_to_root_list(std::list<HeapNode*>* const& list_for_copy);
	//BinomialHeap(const BinomialHeap* second_heap); //TODO
	//BinomialHeap<TKey, TValue>& operator=(const BinomialHeap* second_heap); //TODO
	void print_child(std::list<HeapNode*>* children_ptr, int counter) const;
	void remove_root_list();
	void remove_child(std::list<HeapNode*>* children_ptr);

public:
	static MergeableHeap<TKey, TValue>* create_object(Comparator<TKey>* comparator);
	void merge(const MergeableHeap<TKey, TValue>* heap) override;
	void insert(const TKey&, const TValue&) override;
	std::string who_am_i() const override;
	TValue get_data() const override;
	void print_heap() const override;
	void remove() override;

	~BinomialHeap();
	

};


#pragma region Heap Node implementations
template <typename TKey, typename TValue>
BinomialHeap<TKey, TValue>::HeapNode::HeapNode(const TKey& key, const TValue& value)
{
	degree = 0;
	this->key = key;
	this->value = value;
	this->children_ptr = new std::list<HeapNode*>();
	
}
#pragma endregion

#pragma region TestFunction
template <typename TKey, typename TValue>
std::string BinomialHeap<TKey, TValue>::who_am_i() const
{
	return "Binomial heap.";
}

template <typename TKey, typename TValue>
void BinomialHeap<TKey, TValue>::print_heap() const
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
void BinomialHeap<TKey, TValue>::print_child(std::list<HeapNode*>* children_ptr, int counter) const
{
	counter++;
	for (auto iterator = children_ptr->begin(); iterator != children_ptr->end(); iterator++)
	{
		if (!(*iterator)->children_ptr->empty())
		{
			print_child((*iterator)->children_ptr, counter);
		}
		std::cout << cyan << "Level: " << green << counter << yellow << " [" << (*iterator)->key << " : " << (*iterator)->value << "]," << cyan << " degree =  "  << blue << (*iterator)->degree<< white << std::endl;
	}
}


#pragma endregion

#pragma region Factory
template <typename TKey, typename TValue>
MergeableHeap<TKey, TValue>* BinomialHeap<TKey, TValue>::create_object(Comparator<TKey>* comparator)
{
	return new BinomialHeap<TKey, TValue>(comparator);
}
#pragma endregion

#pragma region Constructor and Destructor and operator=
template <typename TKey, typename TValue>
BinomialHeap<TKey, TValue>::BinomialHeap(Comparator<TKey>* comparator)
{
	this->comparator = comparator;
	extremum_root = nullptr;
}

template <typename TKey, typename TValue>
BinomialHeap<TKey, TValue>::~BinomialHeap()
{
	remove_root_list();
}

template <typename TKey, typename TValue>
void BinomialHeap<TKey, TValue>::remove_root_list()
{
	for (auto iterator = root_list.begin(); iterator != root_list.end(); iterator++)
	{
		remove_child((*iterator)->children_ptr);
		std::cout << red << "Deleting... " << blue << (*iterator)->key << white << std::endl;
		delete (*iterator);
	}
}


template <typename TKey, typename TValue>
void BinomialHeap<TKey, TValue>::remove_child(std::list<HeapNode*>* children_ptr)
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


#pragma region Insertion
template <typename TKey, typename TValue>
void BinomialHeap<TKey, TValue>::push_back_or_front_concat(std::list<HeapNode*>& list, HeapNode* const& old_heap_node, enum Direction dir)
{
	if (old_heap_node == nullptr)
	{
		if (dir == BACK)
		{
			list.pop_back();
			return;
		}
		else if (dir == FRONT)
		{
			list.pop_front();
			return;
		}
	}

	if (dir == BACK)
	{
		list.push_back(old_heap_node);
		return;
	}
	else if (dir == FRONT)
	{
		list.push_front(old_heap_node);
		return;
	}
}


template <typename TKey, typename TValue>
void BinomialHeap<TKey, TValue>::same_degree_concat(HeapNode*& heap_node, HeapNode* const& old_heap_node, std::list<HeapNode*>& list, enum Direction dir)
{
	if (comparator->compare(heap_node->key, old_heap_node->key) > 0)
	{
		heap_node->children_ptr->push_back(old_heap_node);
		heap_node->degree++;
		push_back_or_front_concat(list, nullptr, dir);
		push_back_or_front_concat(list, nullptr, dir);
		push_back_or_front_concat(list, heap_node, dir);
	}
	else
	{
		push_back_or_front_concat(list, nullptr, dir);
		old_heap_node->children_ptr->push_back(heap_node);
		old_heap_node->degree++;
		heap_node = old_heap_node;
	}
}



template <typename TKey, typename TValue>
void BinomialHeap<TKey, TValue>::insert(const TKey& key, const TValue& value)
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

	HeapNode* old_heap_node;
	for (auto iterator = root_list.end(); iterator != root_list.begin(); iterator--)
	{
		if (iterator == root_list.end())
		{
			iterator = std::prev(iterator);
		}
		old_heap_node = *(--iterator);
		if (heap_node->degree == old_heap_node->degree)
		{
			same_degree_concat(heap_node, old_heap_node, root_list, BACK);
			iterator = root_list.end();
			continue;
		}
		break;
	}

	checker_map.insert(std::make_pair(key, key));
}
#pragma endregion

#pragma region Merge
template <typename TKey, typename TValue>
void BinomialHeap<TKey, TValue>::merge(const MergeableHeap<TKey, TValue>* heap)
{
	
	std::list<HeapNode*> merged_list;
	const BinomialHeap<TKey, TValue>* binom_second_heap = reinterpret_cast<const BinomialHeap<TKey, TValue>*>(heap);
	if (root_list.size() == 0) //std::prev(root_list.end()); mistake
	{
		copy_list_to_root_list(&const_cast<BinomialHeap<TKey, TValue>*>(binom_second_heap)->root_list);
		return;
	}
	check_same_keys(binom_second_heap);

	auto self_heap_iter = std::prev(root_list.end());
	auto second_heap_iter = std::prev(binom_second_heap->root_list.end());
	HeapNode* self_node = *self_heap_iter;
	HeapNode* second_node = *second_heap_iter;
	if (comparator->compare(binom_second_heap->extremum_root->key, extremum_root->key) > 0)
	{
		extremum_root = binom_second_heap->extremum_root;
	}


	int i = 0;
	for (; i < min(root_list.size(), binom_second_heap->root_list.size()); i++)
	{
		if (self_node->degree == second_node->degree) // two same degree heaps
		{
			if (comparator->compare(self_node->key, second_node->key) > 0)
			{
				self_node->children_ptr->push_back(second_node);
				self_node->degree++;
				merged_list.push_front(self_node);
			}
			else
			{
				merged_list.push_front(second_node);
				second_node->children_ptr->push_back(self_node);
				second_node->degree++;
			}
		}
		else
		{
			if (second_node->degree < self_node->degree)
			{
				merged_list.push_front(second_node);
				merged_list.push_front(self_node);
			}
			else
			{
				merged_list.push_front(self_node);
				merged_list.push_front(second_node);
			}
		}
		if (second_heap_iter != binom_second_heap->root_list.begin())
		{
			second_heap_iter--;
		}
		if (self_heap_iter != root_list.begin())
		{
			self_heap_iter--;
		}
		self_node = *self_heap_iter;
		second_node = *second_heap_iter;

	}
	bool second_heap_is_longer;
	second_heap_is_longer = (root_list.size() <= binom_second_heap->root_list.size()) ? true : false;
	for (; i < max(root_list.size(), binom_second_heap->root_list.size()); i++)
	{
		auto help_iterator = merged_list.begin();
		if (second_heap_is_longer)
		{
			second_node = *second_heap_iter;
		}
		else
		{
			second_node = *self_heap_iter;
		}
		merged_list.push_front(second_node);
		if ((*help_iterator)->degree == second_node->degree)
		{
			same_degree_concat(second_node, *help_iterator, merged_list, FRONT);
		}
		if (second_heap_is_longer)
		{
			if (second_heap_iter != binom_second_heap->root_list.begin())
			{
				second_heap_iter--;
			}
		}
		else
		{
			if (self_heap_iter != root_list.begin())
			{
				self_heap_iter--;
			}
		}
		
	}

	const_cast<BinomialHeap<TKey, TValue>*>(binom_second_heap)->root_list = std::list<HeapNode*>();
	const_cast<BinomialHeap<TKey, TValue>*>(binom_second_heap)->extremum_root = nullptr;
	root_list = merged_list;
}

template <typename TKey, typename TValue>
void BinomialHeap<TKey, TValue>::check_same_keys(const BinomialHeap<TKey, TValue>* binom_second_heap)
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

#pragma region Getting data
template <typename TKey, typename TValue>
TValue BinomialHeap<TKey, TValue>::get_data() const
{
	if (root_list.empty())
	{
		throw GetEmpty<TKey>("Get data error. Your binomial heap is empty.");
	}
	return extremum_root->value;
}
#pragma endregion

#pragma region Remove
template <typename TKey, typename TValue>
void BinomialHeap<TKey, TValue>::remove()
{
	if (root_list.empty())
	{
		throw RemEmpty<TKey>("Remove data error. Your binomial heap is empty.");
	}

	BinomialHeap<TKey, TValue>* children_binom_heap;
	MergeableHeap<TKey, TValue>* heap_ptr = children_binom_heap->create_object(comparator);
	children_binom_heap = reinterpret_cast<BinomialHeap<TKey, TValue>*>(heap_ptr);
	
	bool deleted = false;
	auto it = root_list.begin();
	while (it != root_list.end())
	{
		if (extremum_root == nullptr)
		{
			extremum_root = *it;
		}
		if (deleted)
		{
			if (comparator->compare((*it)->key, extremum_root->key) > 0)
			{
				extremum_root = *it;
			}
		}
		if ((*it)->key == extremum_root->key && !deleted)
		{
			checker_map.erase(extremum_root->key);
			if ((*it)->children_ptr->empty()) //we don`t have to merge
			{
				delete* it;
				it = root_list.erase(it);
				find_new_extremum_in_list(root_list);
				return;
			}
			children_binom_heap->copy_list_to_root_list((*it)->children_ptr);
			extremum_root = nullptr;
			delete* it;
			it = root_list.erase(it);
			deleted = true;
			continue;
		}
		it++;
	}
	if (extremum_root == nullptr) //while edge case
	{
		find_new_extremum_in_list(root_list);
	}
	this->merge(heap_ptr);
}

template <typename TKey, typename TValue>
void BinomialHeap<TKey, TValue>::find_new_extremum_in_list(std::list<HeapNode*> const& list)
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
void BinomialHeap<TKey, TValue>::copy_list_to_root_list(std::list<HeapNode*>* const& list_for_copy)
{
	auto it = list_for_copy->begin();
	extremum_root = (*it);
	for (; it != list_for_copy->end(); it++)
	{
		if (comparator->compare((*it)->key, extremum_root->key) > 0)
		{
			extremum_root = (*it);
		}
		root_list.push_front((*it));
	}
}
#pragma endregion

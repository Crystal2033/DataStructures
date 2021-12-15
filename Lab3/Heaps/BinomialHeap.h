#pragma once
#include"AbstractHeapClass.h"
#include "Exceptions.h"
#include <list>
#include <map>

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

		void set_new_degree(const int degree) { this->degree = degree; }

	};
private:
	Comparator<TKey>* comparator;
	std::list<HeapNode*> root_list;
	HeapNode* extremum_root;
	std::map<TKey, TKey> checker_map;
	void same_degree_concat(HeapNode*& heap_node, HeapNode* const& old_heap_node, typename std::list<HeapNode*>::iterator& iterator);
	BinomialHeap(Comparator<TKey>* comparator);
	//BinomialHeap(const BinomialHeap* second_heap);


public:
	static MergeableHeap<TKey, TValue>* create_object(Comparator<TKey>* comparator);
	void merge(const MergeableHeap<TKey, TValue>* heap) override;
	void insert(const TKey&, const TValue&) override;
	std::string who_am_i() const override;
	TValue get_data() const override;
	void print_heap() const override;
	void print_child(std::list<HeapNode*>* children_ptr, int counter) const;
	void remove() override;
	~BinomialHeap() = default;
	//BinomialHeap& operator=(BinomialHeap* const& second_heap);

};


#pragma region Heap Node implementations
template <typename TKey, typename TValue>
BinomialHeap<TKey, TValue>::HeapNode::HeapNode(const TKey& key, const TValue& value)
{
	degree = 0;
	this->key = key;
	this->value = value;
	this->children_ptr = new std::list<HeapNode*>;
	
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
	int counter = 0;
	for (auto iterator = root_list.begin(); iterator != root_list.end(); iterator++)
	{
		std::cout << cyan << "Level: " << green << counter << yellow << " [" << (*iterator)->key << " : " << (*iterator)->value << "], " << white << std::endl;
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
		std::cout << cyan << "Level: " << green << counter << yellow << " [" << (*iterator)->key << " : " << (*iterator)->value << "], " << white << std::endl;
	}
	
}


#pragma endregion

#pragma region Factory
template <typename TKey, typename TValue>
MergeableHeap<TKey, TValue>* BinomialHeap<TKey, TValue>::create_object(Comparator<TKey>* comparator)
{
	return new BinomialHeap(comparator);
}
#pragma endregion

#pragma region Constructor and Destructor and operator=
template <typename TKey, typename TValue>
BinomialHeap<TKey, TValue>::BinomialHeap(Comparator<TKey>* comparator)
{
	this->comparator = comparator;
	extremum_root = nullptr;
}
#pragma endregion


#pragma region Insertion
template <typename TKey, typename TValue>
void BinomialHeap<TKey, TValue>::same_degree_concat(HeapNode*& heap_node, HeapNode* const& old_heap_node, typename std::list<HeapNode*>::iterator& iterator)
{
	if (comparator->compare(heap_node->key, old_heap_node->key) > 0)
	{
		heap_node->children_ptr->push_back(old_heap_node);
		heap_node->degree++;
		root_list.pop_back();
		root_list.pop_back();
		root_list.push_back(heap_node);
	}
	else
	{
		root_list.pop_back();
		old_heap_node->children_ptr->push_back(heap_node);
		old_heap_node->degree++;
		heap_node = old_heap_node;
	}
	iterator = root_list.end();
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
			same_degree_concat(heap_node, old_heap_node, iterator);
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

}
#pragma endregion

#pragma region Getting data
template <typename TKey, typename TValue>
TValue BinomialHeap<TKey, TValue>::get_data() const
{
	return extremum_root->value;
}
#pragma endregion

#pragma region Remove
template <typename TKey, typename TValue>
void BinomialHeap<TKey, TValue>::remove()
{

}
#pragma endregion
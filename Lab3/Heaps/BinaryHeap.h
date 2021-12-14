#pragma once
#include"AbstractHeapClass.h"
#include <vector>
#include "Exceptions.h"
#include<map>

template <typename TKey, typename TValue>
class BinaryHeap : public MergeableHeap<TKey, TValue>
{
	class HeapNode
	{
	public:
		TKey key;
		TValue value;
		HeapNode(const TKey& key, const TValue& value);
		HeapNode() = default;

	};
private:
	std::vector<HeapNode*> heap_vector;
	std::map<TKey, TKey> checker_map;
	Comparator<TKey>* comparator;


	BinaryHeap(Comparator<TKey>* comparator);
	BinaryHeap(const BinaryHeap* second_heap);
	
	void heapify(int position);
	
	void make_sift();

public:
	static MergeableHeap<TKey, TValue>* create_object(Comparator<TKey>* comparator);
	void merge(const MergeableHeap<TKey, TValue>* heap) override;
	void insert(const TKey&, const TValue&) override;
	std::string who_am_i() const override;
	TValue get_data() const override;
	void print_heap() const override;
	void remove() override;
	~BinaryHeap();
	BinaryHeap& operator=(BinaryHeap* const& second_heap);

};

#pragma region Heap Node implementations
template <typename TKey, typename TValue>
BinaryHeap<TKey, TValue>::HeapNode::HeapNode(const TKey& key, const TValue& value)
{
	this->key = key;
	this->value = value;
}
#pragma endregion

#pragma region TestFunction
template <typename TKey, typename TValue>
std::string BinaryHeap<TKey, TValue>::who_am_i() const
{
	return "Binary heap.";
}

template <typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::print_heap() const
{
	for (int i = 0; i < heap_vector.size(); i++)
	{
		std::cout << yellow << heap_vector[i]->key << " ";
	}
	std::cout << white << std::endl;
}

#pragma endregion

#pragma region Factory
template <typename TKey, typename TValue>
MergeableHeap<TKey, TValue>* BinaryHeap<TKey, TValue>::create_object(Comparator<TKey>* comparator)
{
	return new BinaryHeap(comparator);
}
#pragma endregion

#pragma region Constructor and Destructor and operator=
template <typename TKey, typename TValue>
BinaryHeap<TKey, TValue>::BinaryHeap(Comparator<TKey>* comparator)
{
	this->comparator = comparator;
}

template <typename TKey, typename TValue>
BinaryHeap<TKey, TValue>::BinaryHeap(const BinaryHeap* second_heap)
{
	this->comparator = second_heap->comparator;
	this->checker_map(second_heap->checker_map);
	this->heap_vector(second_heap->heap_vector);
}

template <typename TKey, typename TValue>
BinaryHeap<TKey, TValue>::~BinaryHeap()
{
	for (int i = heap_vector.size() - 1; i > 0 ; i--)
	{
		checker_map.erase(heap_vector[i]->key);
		delete heap_vector[i];
		heap_vector.pop_back();
	}
}

template <typename TKey, typename TValue>
BinaryHeap<TKey, TValue>& BinaryHeap<TKey, TValue>::operator=(BinaryHeap<TKey, TValue>* const& second_heap)
{
	return BinaryHeap<TKey, TValue>(second_heap);
}
#pragma endregion

#pragma region Insertion
template <typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::insert(const TKey& key, const TValue& value)
{
	auto iterator = this->checker_map.find(key);
	if (iterator != this->checker_map.end())
	{
		throw AddExists<TKey>("Inserting error. Key already exists.", key);
	}

	HeapNode* heap_node = new HeapNode(key, value);
	heap_vector.push_back(heap_node);
	checker_map.insert(std::make_pair(key, key));
	if (heap_vector.size() == 1)
	{
		return;
	}

	make_sift();
	
}
#pragma endregion


#pragma region Heapify
template <typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::heapify(int root_pos)
{
	int largest_val_pos = root_pos;
	int left_child_pos = root_pos * 2 + 1;
	int right_child_pos = root_pos * 2 + 2;

	if (right_child_pos < heap_vector.size() && comparator->compare(heap_vector[right_child_pos]->key, heap_vector[largest_val_pos]->key) > 0)
	{
		largest_val_pos = right_child_pos;
	}

	if (left_child_pos < heap_vector.size() && comparator->compare(heap_vector[left_child_pos]->key, heap_vector[largest_val_pos]->key) > 0)
	{
		largest_val_pos = left_child_pos;
	}

	if (largest_val_pos != root_pos)
	{
		std::swap(heap_vector[root_pos], heap_vector[largest_val_pos]);
		heapify(largest_val_pos);
	}
}

template <typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::merge(const MergeableHeap<TKey, TValue>* heap)
{
	const BinaryHeap<TKey, TValue>* bin_second_heap = reinterpret_cast<const BinaryHeap<TKey, TValue>*>(heap);
	for (int i = 0; i < bin_second_heap->heap_vector.size(); i++)
	{
		auto iterator = checker_map.find(bin_second_heap->heap_vector[i]->key);
		if (iterator != checker_map.end())
		{
			throw MergeSameKeys<TKey>("Impossible to merge two heaps. Because there are two same keys.", bin_second_heap->heap_vector[i]->key);
		}
		checker_map.insert(std::make_pair(bin_second_heap->heap_vector[i]->key, bin_second_heap->heap_vector[i]->key));
		heap_vector.push_back(bin_second_heap->heap_vector[i]);
		make_sift();
	}

}

template <typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::make_sift()
{
	int heap_node_pos = heap_vector.size() - 1;
	int parent_heap_node_pos = (heap_node_pos - 1) / 2;

	while (heap_node_pos > 0 && (comparator->compare(heap_vector[heap_node_pos]->key, heap_vector[parent_heap_node_pos]->key) > 0))
	{
		std::swap(heap_vector[heap_node_pos], heap_vector[parent_heap_node_pos]);
		heap_node_pos = parent_heap_node_pos;
		parent_heap_node_pos = (heap_node_pos - 1) / 2;
	}
}

#pragma endregion


#pragma region Getting data
template <typename TKey, typename TValue>
TValue BinaryHeap<TKey, TValue>::get_data() const
{
	if (heap_vector.size() == 0)
	{
		throw GetEmpty<TKey>("Get data error. Your binary heap is empty.");
	}
	return heap_vector[0]->value;
}
#pragma endregion

#pragma region Removing
template <typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::remove()
{
	if (heap_vector.size() == 0)
	{
		throw RemEmpty<TKey>("Removing from empty heap.");
	}

	heap_vector[0] = heap_vector[heap_vector.size() - 1];
	checker_map.erase(heap_vector[0]->key);
	heap_vector.pop_back();
	heapify(0);
	return;
}
#pragma endregion

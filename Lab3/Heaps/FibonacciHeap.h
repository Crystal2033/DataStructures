#pragma once
#pragma once
#include"AbstractHeapClass.h"

template <typename TKey, typename TValue>
class FibonacciHeap : public MergeableHeap<TKey, TValue>
{
private:
	Comparator<TKey>* comparator;
	FibonacciHeap(Comparator<TKey>* comparator);
public:
	static MergeableHeap<TKey, TValue>* create_object(Comparator<TKey>* comparator);
	void insert(const TKey&, const TValue&) override;
	std::string who_am_i() const override;
	TValue get_data() const override;
	void print_heap() const override {};
	void remove() override;
	
	
	//void merge(MergeableHeap*& heap);
};

#pragma region TestFunction
template <typename TKey, typename TValue>
std::string FibonacciHeap<TKey, TValue>::who_am_i() const
{
	return "Fibonacci Heap.";
}
#pragma endregion

#pragma region Factory
template <typename TKey, typename TValue>
MergeableHeap<TKey, TValue>* FibonacciHeap<TKey, TValue>::create_object(Comparator<TKey>* comparator)
{
	return new FibonacciHeap(comparator);
}
#pragma endregion

#pragma region Constructor
template <typename TKey, typename TValue>
FibonacciHeap<TKey, TValue>::FibonacciHeap(Comparator<TKey>* comparator)
{
	this->comparator = comparator;
}
#pragma endregion

#pragma region Insertion
template <typename TKey, typename TValue>
void FibonacciHeap<TKey, TValue>::insert(const TKey& key, const TValue& value)
{
	//TODO: impementation
	return;
}
#pragma endregion

#pragma region Getting data
template <typename TKey, typename TValue>
TValue FibonacciHeap<TKey, TValue>::get_data() const
{
	//TODO: impementation
	TValue value;
	return value;
}
#pragma endregion

#pragma region Removing
template <typename TKey, typename TValue>
void FibonacciHeap<TKey, TValue>::remove()
{
	//TODO: impementation
	return;
}
#pragma endregion

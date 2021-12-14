#pragma once
#include"Comparator.h"

template <typename TKey, typename TValue>
class MergeableHeap
{
public:
	//static MergeableHeap* create_object(Comparator<TKey>* comparator);
	virtual void merge(const MergeableHeap<TKey, TValue>* heap) = 0;
	virtual void insert(const TKey&, const TValue&) = 0;
	virtual std::string who_am_i() const = 0;
	virtual TValue get_data() const = 0;
	virtual void print_heap() const = 0;
	virtual void remove() = 0;
	
	virtual ~MergeableHeap() = default;

};

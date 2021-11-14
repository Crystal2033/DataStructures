#pragma once
#include "comparer.h"
#include "associative_container.h"
template <typename TKey, typename TData>
class B_Tree : public associative_container<TKey, TData>
{
private:
	class B_Node
	{
	public:
		TKey key[2];
		TData data[2];
		B_Node* pointers[3];
	public:
		B_Node();
		B_Node(const TKey& key, const TData& data);
	};


};
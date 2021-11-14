#pragma once

template <typename TKey, typename TData>
class associative_container
{
public:
	virtual void add(const TKey& key, const TData& data) = 0;
	virtual TData remove(const TKey& key) = 0;
	virtual TData get_data(const TKey& key) const = 0;
	virtual ~associative_container() = default;
};
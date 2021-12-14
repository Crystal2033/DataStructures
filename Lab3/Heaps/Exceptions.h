#pragma once
#include<iostream>
#include<string>

template <typename TKey>
class HeapException : public std::exception
{
protected:
	TKey bad_key;
	std::string error_msg;
public:
	//const char* what() const override;
};

template <typename TKey>
class AddExists : public HeapException<TKey>
{
public:
	AddExists(const std::string& error, const TKey bad_key) {this->error_msg = error; this->bad_key = bad_key;};
	const char* what() const override;
	const TKey get_key() const { return this->bad_key; }
};

#pragma region Existing_what
template <typename TKey>
const char* AddExists<TKey>::what() const
{
	return this->error_msg.c_str();
}
#pragma endregion

template <typename TKey>
class GetEmpty : public HeapException<TKey>
{
public:
	GetEmpty(const std::string& error) { this->error_msg = error; };
	const char* what() const override;
	//const TKey get_key() const { return this->bad_key; }
};

#pragma region Get_from_empty_what
template <typename TKey>
const char* GetEmpty<TKey>::what() const
{
	return this->error_msg.c_str();
}
#pragma endregion

template <typename TKey>
class RemEmpty : public HeapException<TKey>
{
public:
	RemEmpty(const std::string& error) { this->error_msg = error; };
	const char* what() const override;
	//const TKey get_key() const { return this->bad_key; }
};

#pragma region RemEmpty_what
template <typename TKey>
const char* RemEmpty<TKey>::what() const
{
	return this->error_msg.c_str();
}
#pragma endregion


template <typename TKey>
class MergeSameKeys : public HeapException<TKey>
{
public:
	MergeSameKeys(const std::string& error, const TKey bad_key) { this->error_msg = error; this->bad_key = bad_key; };
	const char* what() const override;
	const TKey get_key() const { return this->bad_key; }
};

#pragma region Get_from_empty_what
template <typename TKey>
const char* MergeSameKeys<TKey>::what() const
{
	return this->error_msg.c_str();
}
#pragma endregion


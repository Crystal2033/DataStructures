#pragma once
#include <stack>
#include "comparer.h"
#include "associative_container.h"
#include <cmath>

template <typename TKey, typename TData>
class ClassicBinaryTree : public associative_container<TKey, TData>
{
protected:
	class Node
	{
	public:
		TKey key;
		TData data;
		Node* left_child;
		Node* right_child;
	public:
		Node();
		Node(const TKey& key, const TData& data);
	};

	class rotation_template
	{
		friend class Node;
	public:
		void left_rotation(ClassicBinaryTree::Node*& old_root) const;
		void right_rotation(ClassicBinaryTree::Node*& old_root) const;
	protected:
		virtual void after_left_rotation(ClassicBinaryTree::Node* new_root) const;
		virtual void after_right_rotation(ClassicBinaryTree::Node* new_root) const;
	};
	
	class Make_Rebalance
	{
	public:
		Node* rebalance(std::stack<Node**>& way) const;
	protected:
		virtual void making_balance(Node** &tmp_node, std::stack<Node**>& way) const;
		virtual void clearing(std::stack<Node**>& way, Node**& new_root) const;
	};

	class add_BST
	{
	public:
		virtual void add(Node*& root, comparer<TKey>* comparator, const TKey& key, const TData& data, ClassicBinaryTree<TKey, TData>* const&tree_ptr);
	protected:
		std::stack<Node**> find_way(Node*& root, comparer<TKey>* comparator, const TKey& key) const;
		virtual void allocate_node(Node** to_allocate, comparer<TKey>* const& comparator, const TKey& key, const TData& data, std::stack<Node**>& way) const;
		Make_Rebalance* rebalance_ptr = nullptr;
	public:
		virtual ~add_BST() = default;
	};



	class find_BST
	{
	public:
		virtual TData get_data(Node* const& root, comparer<TKey>* comparator, const TKey& key) const;
	protected:
		TData& find_get(Node* const& root, comparer<TKey>* comparator, const TKey& key) const;
		//virtual TData& splay_hook(Node* const& root, comparer<TKey>* comparator, const TKey& key)
	public:
		virtual ~find_BST() = default;
	};


	class remove_BST
	{
	public:
		virtual TData remove(Node*& root, comparer<TKey>* comparator, const TKey& key, ClassicBinaryTree<TKey, TData>* const& tree_ptr);
		std::stack<Node**> find_way(Node*& root, comparer<TKey>* comparator, const TKey& key) const;
	protected:
		Make_Rebalance* rebalance_ptr = nullptr;
		//std::stack<Node**> find_way(Node*& root, comparer<TKey>* comparator, const TKey& key) const;
		TData remove(std::stack<Node**>&, Node*& root, comparer<TKey>* comparator, const TKey& key, ClassicBinaryTree<TKey, TData>* const& tree_ptr);
		virtual TData remove_leaf(Node*& remove_node, Node*& parent, const TKey& key, std::stack<Node**>&) const;
		virtual TData remove_without_right_branch(Node*& remove_node, Node*& parent, const TKey& key, Node *& root) const;
		virtual TData remove_without_left_branch(Node*& remove_node, Node*& parent, const TKey& key, Node*& root) const;
		virtual void refresh_stack_data(Node*& parent, std::stack<Node**>& way) const;
		virtual void set_black_color_rb(Node*& parent) const;

		virtual void after_delete_balance(std::stack<Node**>& way, typename ClassicBinaryTree<TKey, TData>::Node*& root) const;
	public:
		virtual ~remove_BST() = default;
	};

public:
	class TreeException : public std::exception
	{
	protected:
		std::string _error;
	public:
		TreeException(const std::string& error);
		const char* what() const override;
		virtual ~TreeException() = default;

	};

	class FindNodeError final : public TreeException
	{
	public:
		FindNodeError(const std::string& error, const TKey& key);
		TKey key; //private
		const char* what() const override;
		TKey get_key() const;
		~FindNodeError() = default;
	};

	class KeyExistsError final : public TreeException
	{
	public:
		KeyExistsError(const std::string& error, const TKey &key);
		TKey key;
		TKey get_key() const;
		const char* what() const override;
		~KeyExistsError() = default;
	};

protected:
	Node* root;
	comparer<TKey>* comparator;
protected:
	add_BST* _addition;
	find_BST* _finder;
	remove_BST* _remover;
	rotation_template* _rotator;
	Make_Rebalance* _rebalancer;

protected:
	virtual void copy_for_equal(Node* const& node);
	virtual void add_for_copy(Node* const& copy_from);
	virtual void clean_tree(Node*& node);

protected:
	virtual void prefix(void(*call_back)(const TKey&, const TData&, int), Node* cur_root, int depth = 0) const;
	virtual void infix(void(*call_back)(const TKey&, const TData&, int), Node* cur_root, int depth = 0) const;
	virtual void postfix(void(*call_back)(const TKey&, const TData&, int), Node* cur_root, int depth = 0) const;

public:
	virtual void prefix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const;
	virtual void infix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const;
	virtual void postfix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const;

protected:
	ClassicBinaryTree(comparer<TKey>* cmp, add_BST* inserter, remove_BST* remover, find_BST* finder, rotation_template* rotator, Make_Rebalance* rebalancer);
public:
	ClassicBinaryTree(comparer<TKey>* cmp);
	ClassicBinaryTree(const ClassicBinaryTree<TKey, TData>& from);
	ClassicBinaryTree& operator=(const ClassicBinaryTree<TKey, TData>& value);
	virtual ~ClassicBinaryTree();

public:
	void add(const TKey& key, const TData& data) override final;
	TData remove(const TKey& key) override final;
	TData get_data(const TKey& key) const override final;
};

#pragma region Node implementations

template<typename TKey, typename TData>
ClassicBinaryTree<TKey, TData>::Node::Node()
{
	this->left_child = nullptr;
	this->right_child = nullptr;
}

template<typename TKey, typename TData>
ClassicBinaryTree<TKey, TData>::Node::Node(const TKey& key, const TData& data)
{
	this->key = key;
	this->data = data;
	this->left_child = nullptr;
	this->right_child = nullptr;
}

#pragma endregion

#pragma region rotation_template implementations

template<typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::rotation_template::left_rotation(ClassicBinaryTree::Node*& old_root) const
{
	if (old_root == nullptr)
	{
		// TODO: exception
		return;
	}

	if (old_root->right_child == nullptr)
	{
		// TODO: exception
		return;
	}

	typename ClassicBinaryTree<TKey, TData>::Node* old_root_temp = old_root;
	old_root = old_root->right_child;
	old_root_temp->right_child = old_root->left_child;
	old_root->left_child = old_root_temp;

	after_left_rotation(old_root);
}

template<typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::rotation_template::after_left_rotation(ClassicBinaryTree::Node* new_root) const
{

}

template<typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::rotation_template::right_rotation(ClassicBinaryTree::Node*& old_root) const
{
	if (old_root == nullptr)
	{
		// TODO: exception
		return;
	}

	if (old_root->left_child == nullptr)
	{
		// TODO: exception
		return;
	}

	typename ClassicBinaryTree<TKey, TData>::Node* old_root_temp = old_root;
	old_root = old_root->left_child;
	old_root_temp->left_child = old_root->right_child;
	old_root->right_child = old_root_temp;

	after_right_rotation(old_root);
}

template<typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::rotation_template::after_right_rotation(ClassicBinaryTree::Node* new_root) const
{

}
#pragma endregion

#pragma region add_BST implementations

template <typename TKey, typename TData>
std::stack<typename ClassicBinaryTree<TKey, TData>::Node**> ClassicBinaryTree<TKey, TData>::add_BST::find_way(Node*& root, comparer<TKey>* comparator, const TKey& key) const
{
	std::stack<Node**> stack;
	if (root == nullptr)
	{
		return stack; //вернем пустой стек.
	}
	else
	{
		Node** tmp = &root;
		stack.push(tmp);
		while (tmp)
		{
			int compare_result = comparator->compare(key, (*tmp)->key);
			if (compare_result > 0)
			{
				if ((*tmp)->right_child == nullptr)
				{
					return stack;
				}
				tmp = &((*tmp)->right_child);
				stack.push(tmp);
			}
			else if (compare_result == 0)
			{
				throw ClassicBinaryTree::KeyExistsError("Key is already exists.", (*tmp)->key);
			}
			else
			{
				if ((*tmp)->left_child == nullptr)
				{
					return stack;
				}
				tmp = &((*tmp)->left_child);
				stack.push(tmp);
			}
		}
	}
	return stack;
}

template <typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::add_BST::add(Node*& root, comparer<TKey>* comparator, const TKey& key, const TData& data, ClassicBinaryTree<TKey, TData>* const& tree_ptr)
{
	std::stack<Node**> stack_way = this->find_way(root, comparator, key);
	if (stack_way.empty())
	{
		allocate_node(&root, comparator, key, data, stack_way);
		rebalance_ptr = tree_ptr->_rebalancer;
		return;
	}
	Node** last_node = stack_way.top();
	
	allocate_node(last_node, comparator, key, data, stack_way);
	root = rebalance_ptr->rebalance(stack_way);
}

template <typename TKey, typename TData>
void  ClassicBinaryTree<TKey, TData>::add_BST::allocate_node(Node** to_allocate, comparer<TKey>* const& comparator, const TKey& key, const TData& data, std::stack<Node**>& way) const
{
	if (*to_allocate == nullptr) //for root
	{
		*to_allocate = new Node(key, data);
		way.push(to_allocate);
		return;
	}
	if (comparator->compare(key, (*to_allocate)->key) > 0) //Значит, ушел вправо --> добавляем в правое поддерево.
	{
		(*to_allocate)->right_child = new Node(key, data);
		way.push((&(*to_allocate)->right_child));
	}
	else
	{
		(*to_allocate)->left_child = new Node(key, data);
		way.push((&(*to_allocate)->left_child));
	}
}

#pragma endregion

#pragma region find_BST implementations

//template <typename TKey, typename TData>
//std::stack<typename ClassicBinaryTree<TKey, TData>::Node*> ClassicBinaryTree<TKey, TData>::find_BST::find_way(Node* const& root, comparer<TKey>* comparator, const TKey& key) const
//{
//	std::stack<Node*> stack;
//	if (root == nullptr)
//	{
//		throw ClassicBinaryTree::FindNodeError("Tree is empty.", key);
//	}
//	else
//	{
//		Node* tmp = const_cast<Node*>(root);
//		stack.push(tmp);
//		while (tmp)
//		{
//			switch (comparator->compare(key, tmp->key))
//			{
//			case 1:
//			{
//				if (tmp->right_child == nullptr)
//				{
//					break;
//				}
//				tmp = tmp->right_child;
//				stack.push(tmp);
//				break;
//			}
//			case 0:
//			{
//				return stack;
//			}
//			case -1:
//			{
//				if (tmp->left_child == nullptr)
//				{
//					break;
//				}
//				tmp = tmp->left_child;
//				stack.push(tmp);
//				break;
//			}
//			}
//		}
//		throw ClassicBinaryTree::FindNodeError("Node with your key wasn`t found.", key);
//	}
//	return stack;
//}

template <typename TKey, typename TData>
TData& ClassicBinaryTree<TKey, TData>::find_BST::find_get(Node* const& root, comparer<TKey>* comparator, const TKey& key) const
{
	if (root == nullptr)
	{
		throw ClassicBinaryTree::FindNodeError("Tree is empty.", key);
	}
	else
	{
		Node* tmp = const_cast<Node*>(root);
		while (tmp)
		{
			int compare_result = comparator->compare(key, tmp->key);
			if (compare_result > 0)
			{
				tmp = tmp->right_child;
			}
			else if (compare_result == 0)
			{
				return tmp->data;
			}
			else
			{
				tmp = tmp->left_child;
			}
		}

		throw ClassicBinaryTree::FindNodeError("Node with your key wasn`t found.", key);
	}

}

template <typename TKey, typename TData>
TData ClassicBinaryTree<TKey, TData>::find_BST::get_data(Node* const& root, comparer<TKey>* comparator, const TKey& key) const
{
	return find_get(root, comparator, key); //представим, что тут еще для splay, получается надо найти что-то, а потом опеять же найти это в splay и вернуть.
}

#pragma endregion

#pragma region remove_BST implementations

template<typename TKey, typename TData>
TData ClassicBinaryTree<TKey, TData>::remove_BST::remove(Node*& root, comparer<TKey>* comparator, const TKey& key, ClassicBinaryTree<TKey, TData>* const& tree_ptr)
{
	this->rebalance_ptr = tree_ptr->_rebalancer;
	std::stack<Node**> way = find_way(root, comparator, key);
	TData removed_data = remove(way, root, comparator, key, tree_ptr); //for recusion tree_ptr
	after_delete_balance(way, root);
	return removed_data;
	//return remove(way, root, comparator, key);
}

template<typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::remove_BST::after_delete_balance(std::stack<Node**>& way, typename ClassicBinaryTree<TKey, TData>::Node*& root) const
{

}

template<typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::remove_BST::refresh_stack_data(Node*& parent, std::stack<Node**>& way) const
{

}

template<typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::remove_BST::set_black_color_rb(Node*& parent) const
{

}

template <typename TKey, typename TData>
std::stack<typename ClassicBinaryTree<TKey, TData>::Node**> ClassicBinaryTree<TKey, TData>::remove_BST::find_way(Node*& root, comparer<TKey>* comparator, const TKey& key) const
{
	std::stack<Node**> stack;
	if (root == nullptr)
	{
		throw ClassicBinaryTree::FindNodeError("Tree is empty.", key);
	}
	else
	{
		Node** tmp = &root;
		stack.push(tmp);
		while (tmp)
		{

			int compare_result = comparator->compare(key, (*tmp)->key);
			if (compare_result > 0)
			{
				if ((*tmp)->right_child == nullptr)
				{
					break;
				}
				tmp = &((*tmp)->right_child);
				stack.push(tmp);
			}
			else if (compare_result == 0)
			{
				return stack;
			}
			else
			{
				if ((*tmp)->left_child == nullptr)
				{
					break;
				}
				tmp = &((*tmp)->left_child);
				stack.push(tmp);
			}
		}
		throw ClassicBinaryTree::FindNodeError("Node with your key wasn`t found.", key);
	}
	return stack;
}

template <typename TKey, typename TData>
TData ClassicBinaryTree<TKey, TData>::remove_BST::remove_leaf(Node*& remove_node, Node*& parent, const TKey& key, std::stack<Node**>& way) const
{
	TData rem_data;
	if (parent == nullptr)
	{
		//remove_node->left_child = nullptr;
		rem_data = remove_node->data;
		delete remove_node;
		return rem_data;
	}
	if (parent->left_child != nullptr && parent->left_child->key == key)
	{
		parent->left_child = nullptr;
	}
	else if (parent->right_child != nullptr && parent->right_child->key == key)
	{
		parent->right_child = nullptr;
	}

	rem_data = remove_node->data;
	delete remove_node;
	return rem_data;
}

template <typename TKey, typename TData>
TData ClassicBinaryTree<TKey, TData>::remove_BST::remove_without_right_branch(Node*& remove_node, Node*& parent, const TKey& key, Node*& root) const
{
	TData rem_data;
	
	if (parent == nullptr)
	{
		if (remove_node->left_child == nullptr || remove_node->right_child == nullptr) 
		{
			root = remove_node->left_child;
			//set_black_color_rb(remove_node->left_child);
			remove_node->left_child = nullptr;
			rem_data = remove_node->data;
			delete remove_node;
			return rem_data;
		}
	}

	if (parent->left_child == remove_node)
	{
		parent->left_child = remove_node->left_child;
		remove_node->left_child = nullptr;

		rem_data = remove_node->data;
		delete remove_node;
		return rem_data;
	}
	else if (parent->right_child == remove_node)
	{
		parent->right_child = remove_node->left_child;
		remove_node->left_child = nullptr;

		rem_data = remove_node->data;
		delete remove_node;
		return rem_data;
	}
}


template <typename TKey, typename TData>
TData ClassicBinaryTree<TKey, TData>::remove_BST::remove_without_left_branch(Node*& remove_node, Node*& parent, const TKey& key, Node*& root) const
{
	TData rem_data;
	if (parent == nullptr)
	{
		if (remove_node->left_child == nullptr || remove_node->right_child == nullptr)
		{
			root = remove_node->right_child;
			set_black_color_rb(remove_node->right_child);
			remove_node->right_child = nullptr;
			rem_data = remove_node->data;
			delete remove_node;
			return rem_data;
		}
	}
	if (parent->left_child == remove_node)
	{
		parent->left_child = remove_node->right_child;
		remove_node->right_child = nullptr;

		rem_data = remove_node->data;
		delete remove_node;
		return rem_data;
	}
	else if (parent->right_child == remove_node)
	{
		parent->right_child = remove_node->right_child;
		remove_node->right_child = nullptr;

		rem_data = remove_node->data;
		delete remove_node;
		return rem_data;
	}
}
//remove_without_right_branch
template <typename TKey, typename TData>
TData ClassicBinaryTree<TKey, TData>::remove_BST::remove(std::stack<Node**>& way, Node*& root, comparer<TKey>* comparator, const TKey& key, ClassicBinaryTree<TKey, TData>* const& tree_ptr)
{
	Node* remove_node;
	Node* parent = nullptr;
	remove_node = *(way.top());
	way.pop();
	if (!way.empty())
	{
		parent = *(way.top());
	}
	TData rem_data;

	if (!remove_node->left_child && !remove_node->right_child) // 1 случай. ЛИСТ. Нет детей.
	{
		rem_data = remove_leaf(remove_node, parent, key, way);
		if (parent == nullptr)
		{
			root = nullptr;
			return rem_data;
		}
		if (!way.empty())
		{
			Node** tmp_parent = way.top();
			*tmp_parent = parent;
		}
		return rem_data;
		/*if (parent->left_child != nullptr && parent->left_child->key == key)
		{
			parent->left_child = nullptr;
		}
		else if (parent->right_child != nullptr && parent->right_child->key == key)
		{
			parent->right_child = nullptr;
		}

		rem_data = remove_node->data;
		delete remove_node;
		return rem_data;*/
	}
	else if ((remove_node->left_child != nullptr && remove_node->right_child == nullptr) || (remove_node->left_child == nullptr && remove_node->right_child != nullptr)) //2 Случай. Когда ЕСТЬ ОДНО ВЕТВЛЕНИЕ. 1 ребёнок у удаляемого.
	{
		if ((remove_node->left_child != nullptr && remove_node->right_child == nullptr)) //У удаляемого нет правой ветки.
		{
			rem_data = remove_without_right_branch(remove_node, parent, key, root);
			return rem_data;
			/*if (parent->left_child == remove_node)
			{
				parent->left_child = remove_node->left_child;
				remove_node->left_child = nullptr;

				rem_data = remove_node->data;
				delete remove_node;
				return rem_data;
			}
			else if (parent->right_child == remove_node)
			{
				parent->right_child = remove_node->left_child;
				remove_node->left_child = nullptr;

				rem_data = remove_node->data;
				delete remove_node;
				return rem_data;
			}*/
		}
		else if (remove_node->left_child == nullptr && remove_node->right_child != nullptr) // У удаляемого нет левой ветки.
		{
			rem_data = remove_without_left_branch(remove_node, parent, key, root);
			return rem_data;
			/*if (parent->left_child == remove_node)
			{
				parent->left_child = remove_node->right_child;
				remove_node->right_child = nullptr;

				rem_data = remove_node->data;
				delete remove_node;
				return rem_data;
			}
			else if (parent->right_child == remove_node)
			{
				parent->right_child = remove_node->right_child;
				remove_node->right_child = nullptr;

				rem_data = remove_node->data;
				delete remove_node;
				return rem_data;
			}*/
		}

	}
	else // У удаляемого есть обе ветки.
	{
		Node* maximal_right_node = remove_node->left_child;
		Node* maximal_right_node_parent = remove_node;
		while (maximal_right_node->right_child)
		{
			maximal_right_node_parent = maximal_right_node;
			maximal_right_node = maximal_right_node->right_child;
		}
		TKey maximal_right_node_key = maximal_right_node->key;
		TData maximal_right_node_data = maximal_right_node->data;
		rem_data = remove_node->data;
		way = find_way(root, comparator, maximal_right_node_key);
		remove_node->data = maximal_right_node_data;
		remove_node->key = maximal_right_node_key;
		
		if (maximal_right_node->right_child == nullptr && maximal_right_node->left_child == nullptr)
		{
			remove_leaf(maximal_right_node, maximal_right_node_parent, maximal_right_node_key, way);
			if (maximal_right_node_parent == nullptr)
			{ 
				root = nullptr;
				return rem_data;
			}
			
			return rem_data;
		}
		else if (maximal_right_node->left_child != nullptr && maximal_right_node->right_child == nullptr)
		{
			remove_without_right_branch(maximal_right_node, maximal_right_node_parent, maximal_right_node_key, root);
			return rem_data;
		}
		else if(maximal_right_node->left_child == nullptr && maximal_right_node->right_child != nullptr)
		{
			remove_without_left_branch(maximal_right_node, maximal_right_node_parent, maximal_right_node_key, root);
			return rem_data;
		}
	}
}

#pragma endregion

#pragma region exceptions

template<typename TKey, typename TData>
ClassicBinaryTree<TKey, TData>::TreeException::TreeException(const std::string& error)
	: _error(error)
{

}

template<typename TKey, typename TData>
const char* ClassicBinaryTree<TKey, TData>::TreeException::what() const
{
	return _error.c_str();
}

template<typename TKey, typename TData>
ClassicBinaryTree<TKey, TData>::FindNodeError::FindNodeError(const std::string& error, const TKey& key)
	: TreeException(error)
{
	this->key = key;
}

template<typename TKey, typename TData>
const char* ClassicBinaryTree<TKey, TData>::FindNodeError::what() const
{
	return TreeException::_error.c_str();
}


template<typename TKey, typename TData>
TKey ClassicBinaryTree<TKey, TData>::FindNodeError::get_key() const
{
	return key;
}

//template<typename TKey, typename TData>
//ClassicBinaryTree<TKey, TData>::KeyExistsError::KeyExistsError(const std::string& error)
//	: TreeException(error)
//{
//	
//}

template<typename TKey, typename TData>
ClassicBinaryTree<TKey, TData>::KeyExistsError::KeyExistsError(const std::string& error, const TKey& _key)
	: TreeException(error), key(_key)
{

}


template<typename TKey, typename TData>
TKey ClassicBinaryTree<TKey, TData>::KeyExistsError::get_key() const
{
	return key;
}


template<typename TKey, typename TData>
const char* ClassicBinaryTree<TKey, TData>::KeyExistsError::what() const
{
	return TreeException::_error.c_str();
}

#pragma endregion

#pragma region miscellaneous methods

template <typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::add_for_copy(Node* const& copy_from)
{
	if (root == nullptr)
	{
		root = new Node(copy_from->key, copy_from->data);
		return;
	}
	else
	{
		Node* tmp = root;

		while (tmp)
		{
			switch (comparator->compare(copy_from->key, tmp->key))
			{
			case 1:
				if (tmp->right_child == nullptr)
				{
					tmp->right_child = new Node(copy_from->key, copy_from->data);
					tmp = tmp->right_child; //new
				}
				tmp = tmp->right_child;
				break;
			case 0:
				throw KeyExistsError("Impossible to push node in the tree. The key already exists.", tmp->key);
			case -1:
				if (tmp->left_child == nullptr)
				{
					tmp->left_child = new Node(copy_from->key, copy_from->data);
					tmp = tmp->left_child; // new
				}
				tmp = tmp->left_child;
				break;
			}
		}
	}
	return;
}

template <typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::copy_for_equal(Node* const& node)
{
	add_for_copy(node);
	if (node->right_child != nullptr)
	{
		copy_for_equal(node->right_child);
	}
	if (node->left_child != nullptr)
	{
		copy_for_equal(node->left_child);
	}
	return;
}

template <typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::clean_tree(Node*& node)
{
	if (node != nullptr)
	{
		clean_tree(node->left_child);
		clean_tree(node->right_child);
		delete node;
	}
	//root = nullptr;
}

// TODO: remove this from release version, traverses are better for this
template <typename TKey, typename TData>
void print_tree(const TKey& key, const TData& data, int depth = 0)
{
	if (depth == 0)
	{
		std::cout << red << "ROOT: " << white << " ";
	}
	else
	{
		std::cout << azure << "Depth: " << depth << "." << white << " ";
	}
	std::cout << green << "Key: " << key << "." << white << " ";
	std::cout << blue << "Data: " << data << "." << white << std::endl;
}

#pragma endregion

#pragma region traverse methods

template <typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::prefix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const 
{
	prefix(call_back, this->root);
	return;
}

template <typename TKey, typename TData>
void  ClassicBinaryTree<TKey, TData>::infix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const
{
	infix(call_back, this->root);
	return;
}

template <typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::postfix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const
{
	postfix(call_back, this->root);
	return;
}

template <typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::prefix(void(*call_back)(const TKey&, const TData&, int), Node* cur_root, int depth) const
{
	if (cur_root != nullptr)
	{
		call_back(cur_root->key, cur_root->data, depth++);
		//depth++;
		prefix(call_back, cur_root->left_child, depth);
		prefix(call_back, cur_root->right_child, depth--);
		//depth--;
	}
	return;
}

template <typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::infix(void(*call_back)(const TKey&, const TData&, int), Node* cur_root, int depth) const
{
	if (cur_root != nullptr)
	{
		//depth++;
		infix(call_back, cur_root->left_child, ++depth);
		//depth--;
		call_back(cur_root->key, cur_root->data, --depth);
		//depth++;
		infix(call_back, cur_root->right_child, ++depth);
	}
	return;
}

template <typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::postfix(void(*call_back)(const TKey&, const TData&, int), Node* cur_root, int depth) const
{
	if (cur_root != nullptr)
	{
		//depth++;
		postfix(call_back, cur_root->left_child, ++depth);
		postfix(call_back, cur_root->right_child, depth--);
		//depth--;

		call_back(cur_root->key, cur_root->data, depth++);
		//depth++;
	}
	return;
}

#pragma endregion

#pragma region initialization, assignment, destruction

template <typename TKey, typename TData>
ClassicBinaryTree<TKey, TData>::ClassicBinaryTree(comparer <TKey>* cmp, add_BST* inserter, remove_BST* remover, find_BST* finder, rotation_template* rotator, Make_Rebalance* rebalancer)
	: root(nullptr), comparator(cmp), _addition(inserter), _remover(remover), _finder(finder), _rotator(rotator), _rebalancer(rebalancer)
{

}

template <typename TKey, typename TData>
ClassicBinaryTree<TKey, TData>::ClassicBinaryTree(comparer <TKey>* cmp)
	: ClassicBinaryTree(cmp, new add_BST, new remove_BST, new find_BST, new rotation_template, new Make_Rebalance)
{

}

template <typename TKey, typename TData>
ClassicBinaryTree<TKey, TData>::ClassicBinaryTree(const ClassicBinaryTree<TKey, TData>& from)
	: comparator(from.comparator), _addition(new add_BST), _remover(new remove_BST), _finder(new find_BST), _rotator(new rotation_template), _rebalancer(new Make_Rebalance)
{
	copy_for_equal(from.root);
}

template <typename TKey, typename TData>
ClassicBinaryTree<TKey, TData>& ClassicBinaryTree<TKey, TData>::operator=(const ClassicBinaryTree<TKey, TData>& value)
{
	clean_tree(root);
	root = nullptr;
	comparator = value.comparator;
	copy_for_equal(value.root);

	return *this;
}

template <typename TKey, typename TData>
ClassicBinaryTree<TKey, TData>::~ClassicBinaryTree()
{
	clean_tree(root);
	delete _addition;
	delete _finder;
	delete _remover;
	delete _rotator;
	delete _rebalancer;
};

#pragma endregion

#pragma region associative_container overrides

template <typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::add(const TKey& key, const TData& data)
{
	_addition->add(this->root, comparator, key, data, this);
}
template <typename TKey, typename TData>
TData ClassicBinaryTree<TKey, TData>::remove(const TKey& key)
{
	return _remover->remove(this->root, comparator, key, this);
}
template <typename TKey, typename TData>
TData ClassicBinaryTree<TKey, TData>::get_data(const TKey& key) const
{
	return _finder->get_data(root, comparator, key);
}

#pragma endregion


#pragma region MakeRebalance template method
template <typename TKey, typename TData>
typename ClassicBinaryTree<TKey, TData>::Node* ClassicBinaryTree<TKey, TData>::Make_Rebalance::rebalance(std::stack<Node**>& way) const
{
	Node** tmp_node = nullptr;
	while (!way.empty())
	{
		making_balance(tmp_node, way);
		clearing(way, tmp_node);
	}
	return *tmp_node;
}
template <typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::Make_Rebalance::clearing(std::stack<Node**>& way, Node**& new_root) const
{
	way.pop();
}

template<typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::Make_Rebalance::making_balance(Node**& tmp_node, std::stack<Node**>& way) const
{
	tmp_node = way.top();
}
#pragma endregion 

#pragma once
#include "Comparator.h"
#include <map>
template <typename TKey, typename TData>
class Container
{
protected:
	virtual void add(const TKey& node, const TData& data) = 0;
	virtual TData remove(const TKey& key) = 0;
	virtual TData get_data(const TKey& key) const = 0;
};

template <typename TKey, typename TData>
class ClassicBinaryTree : protected Container<TKey, TData>
{
protected:
	class Node
	{
	public:
		TKey key;
		TData data;
		Node* left_child;
		Node* right_child;
		Node()
		{
			this->left_child = nullptr;
			this->right_child = nullptr;
		}
		Node(const TKey& key, const TData& data)
		{
			this->key = key;
			this->data = data;
			this->left_child = nullptr;
			this->right_child = nullptr;
		}

	};

	Node* root;
	Comparator<TKey>* comparator;

	void prefix(void(*call_back)(const TKey&, const TData&, int), Node* cur_root, int depth = 0) const;
	void infix(void(*call_back)(const TKey&, const TData&, int), Node* cur_root, int depth = 0) const;
	void postfix(void(*call_back)(const TKey&, const TData&, int), Node* cur_root, int depth = 0) const;

	void copy_for_equal(Node* const& node);
	void add_for_copy(Node* const& copy_from);
	void clean_tree(Node*& node);

public:
	ClassicBinaryTree()
	{
		root = nullptr;
		this->comparator = nullptr;
	}
	ClassicBinaryTree(Comparator <TKey>* cmp)
	{
		comparator = cmp;
		root = nullptr;
	}

	ClassicBinaryTree(const ClassicBinaryTree<TKey, TData>& r_value)
	{
		this->comparator = r_value.comparator;
		copy_for_equal(r_value.root);

	}
	 ~ClassicBinaryTree()
	{
		clean_tree(root);
	};
	
	 class add_BST
	 {
	 private:
		 void add(const TKey& key, const TData& data);
	 };
	 class find_BST
	 {
	 private:
		 TData get_data(const TKey& key) const;
	 };
	 class remove_BST
	 {
	 private:
		 TData remove(const TKey& key);
	 };

	 void add(const TKey& key, const TData& data) override
	 {
		 return this->add_BST::add(key, data);
	 }
	 TData remove(const TKey& key) override
	 {
		 return this->remove_BST::remove(key);
	 }
	 TData get_data(const TKey& key) const override
	 {
		 return this->find_BST::get_data(key);
	 }

	/*void add(const TKey& key, const TData& data) override;
	TData remove(const TKey& key) override;
	TData get_data(const TKey& key) const override;*/

	ClassicBinaryTree& operator=(const ClassicBinaryTree<TKey, TData>& value);

	void prefix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const;
	void infix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const ;
	void postfix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const;


	class TreeException : public std::exception
	{
	protected:
		std::string error;
	public:
		/*TreeException()
		{

		}*/
		TreeException(const std::string& error)
		{
			this->error = error;
		}

		const char* what() const override
		{
			return this->error.c_str();
		}
	};

	class FindNodeError : public TreeException
	{
	public:

		FindNodeError(const std::string& error)
		{
			this->error = error;
		}

		const char* what() const override
		{
			return this->error.c_str();
		}
	};

	class KeyExistsError : public TreeException
	{
	public:
		KeyExistsError(const std::string& error)
		{
			this->error = error;
		}
		const char* what() const override
		{
			return this->error.c_str();
		}
	};
};


template <typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::add_BST::add(const TKey& key, const TData& data)
{
	if (root == nullptr)
	{
		root = new Node(key, data);
	}
	else
	{
		Node* tmp = root;

		while (tmp)
		{
			switch (comparator->compare(key, tmp->key))
			{
			case 1:
			{
				if (tmp->right_child == nullptr)
				{
					tmp->right_child = new Node(key, data);
					tmp = tmp->right_child;
				}
				tmp = tmp->right_child;
				break;
			}
			case 0:
			{
				throw KeyExistsError("Impossible to push node in the tree. The key already exists.");
			}
			case -1:
			{
				if (tmp->left_child == nullptr)
				{
					tmp->left_child = new Node(key, data);
					tmp = tmp->left_child;
				}
				tmp = tmp->left_child;
				break;
			}
			}
		}
	}
}

template <typename TKey, typename TData>
typename TData ClassicBinaryTree<TKey, TData>::find_BST::get_data(const TKey& key) const
{
	if (root == nullptr)
	{
		throw FindNodeError("Impossible to find node. Tree is empty.");
	}

	else
	{
		Node* tmp = root;
		while (tmp)
		{
			switch (comparator->compare(key, tmp->key))
			{
			case 1:
			{
				tmp = tmp->right_child;
				break;
			}
			case 0:
			{
				return tmp->data;
			}
			case -1:
			{
				tmp = tmp->left_child;
				break;
			}
			}
		}

		throw FindNodeError("Node wasn`t found.");

	}
}


template <typename TKey, typename TData>
TData ClassicBinaryTree<TKey, TData>::remove_BST::remove(const TKey& key)
{
	if (root == nullptr)
	{
		throw FindNodeError("Impossible to find node. Tree is empty.");
	}
	Node* parent, * remove_node;

	remove_node = root;
	parent = remove_node;

	while ((remove_node != nullptr) && remove_node->key != key) //Цикл действует, пока не найден ключ и не нулл. Что-то одно -- 0 => break;
	{
		parent = remove_node;
		switch (comparator->compare(key, remove_node->key))
		{
		case 1:
		{
			remove_node = remove_node->right_child;
			break;
		}
		case 0:
		{
			break;
		}

		case -1:
		{
			remove_node = remove_node->left_child;
			break;
		}
		default:
			break;
		}
	}
	if (!remove_node)
	{
		throw FindNodeError("Node wasn`t found.");
	}
	TData rem_data;

	if (!remove_node->left_child && !remove_node->right_child) // 1 случай. ЛИСТ. Нет детей.
	{
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
	else if ((remove_node->left_child != nullptr && remove_node->right_child == nullptr) || (remove_node->left_child == nullptr && remove_node->right_child != nullptr)) //2 Случай. Когда ЕСТЬ ОДНО ВЕТВЛЕНИЕ. 1 ребёнок у удаляемого.
	{
		if ((remove_node->left_child != nullptr && remove_node->right_child == nullptr)) //У ребёнка нет правой ветки.
		{
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
		else if (remove_node->left_child == nullptr && remove_node->right_child != nullptr) // У ребёнка нет левой ветки.
		{
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

	}
	else // У ребенка есть обе ветки.
	{
		Node* minimal_left_node = remove_node->right_child;
		while (minimal_left_node->left_child)
		{
			minimal_left_node = minimal_left_node->left_child;
		}
		TKey minimal_left_node_key = minimal_left_node->key;
		TData minimal_left_node_data = minimal_left_node->data;
		rem_data = remove(minimal_left_node_key);
		remove_node->data = minimal_left_node_data;
		remove_node->key = minimal_left_node_key;
		return rem_data;
	}

}

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
			{
				if (tmp->right_child == nullptr)
				{
					tmp->right_child = new Node(copy_from->key, copy_from->data);
					tmp = tmp->right_child; //new
				}
				tmp = tmp->right_child;
				break;
			}
			case 0:
			{
				throw Exception("Impossible to push node in the tree. The key already exists.", tmp->key);
			}
			case -1:
			{
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
}

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

template <typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::prefix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const 
{
	prefix(call_back, root);
	return;
}


template <typename TKey, typename TData>
void  ClassicBinaryTree<TKey, TData>::infix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const
{
	infix(call_back, root);
	return;
}


template <typename TKey, typename TData>
void ClassicBinaryTree<TKey, TData>::postfix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const
{
	postfix(call_back, root);
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

template <typename TKey, typename TData>
ClassicBinaryTree<TKey, TData>& ClassicBinaryTree<TKey, TData>::operator=(const ClassicBinaryTree<TKey, TData>& value)
{
	clean_tree(root);
	root = nullptr;
	this->comparator = value.comparator;
	copy_for_equal(value.root);

	return *this;
}


template <typename TKey, typename TData>
class AVL_Tree final : public ClassicBinaryTree<TKey, TData>
{
public:
	class AVL_Node final : public ClassicBinaryTree<TKey, TData>::Node
	{
	public:
		int height;
		AVL_Node* left_child;
		AVL_Node* right_child;
		AVL_Node()
		{

		}
		AVL_Node(const TKey& key, const TData& data)
		{
			this->key = key;
			this->data = data;
			this->left_child = nullptr;
			this->right_child = nullptr;
			this->height = 0;
		}
		
	};
private:
	AVL_Node* root;
	int get_height(AVL_Node*& node);
	void clean_tree(AVL_Node*& node);
	void prefix(void(*call_back)(const TKey&, const TData&, int), AVL_Node* cur_root, int depth = 0) const;
	void postfix(void(*call_back)(const TKey&, const TData&, int), AVL_Node* cur_root, int depth = 0) const;
	void infix(void(*call_back)(const TKey&, const TData&, int), AVL_Node* cur_root, int depth = 0) const;

public:
	AVL_Tree()
	{
		this->root = nullptr;
	}
	AVL_Tree(Comparator <TKey>* cmp)
	{
		this->comparator = cmp;
		this->root = nullptr;
	}

	AVL_Tree(const AVL_Tree<TKey, TData>& tree)
	{
		this->comparator = tree.comparator;
		this->copy_for_equal(tree.root);
	}
	~AVL_Tree()
	{
		clean_tree(root);
	}
	void prefix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const;
	void infix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const;
	void postfix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const;

	class AVL_KeyExistsException : protected ClassicBinaryTree<TKey, TData>::TreeException
	{
	private:
		std::string error;
	public:
		AVL_KeyExistsException(const std::string& error)
		{
			this->error = error;
		}
		const char* what() const override
		{
			return this->error.c_str();
		}
	};

	void add(const TKey& key, const TData& data) override;
	/*TData remove(const TKey& key) override;
	TData get_data(const TKey& key) const override;*/
	
};

template <typename TKey, typename TData>
int AVL_Tree<TKey, TData>::get_height(AVL_Node*& node)
{
	if (node == nullptr)
	{ 
		return -1;
	}
	
	int left_height, right_heigth;
	left_height = get_height(node->left_child);
	right_heigth = get_height(node->right_child);
	if (left_height > right_heigth)
	{
		return left_height + 1;
	}
	else
	{
		return right_heigth + 1;
	}
}

template <typename TKey, typename TData>
void AVL_Tree<TKey, TData>::add(const TKey& key, const TData& data)
{
	if (!this->root)
	{
		this->root = new AVL_Node(key, data);
		return;
	}
	AVL_Node* tmp = this->root;

	while (tmp)
	{
		switch (this->comparator->compare(key, tmp->key))
		{
			case -1:
			{
				if (!tmp->left_child)
				{
					tmp->left_child = new AVL_Node(key, data);
					//tmp->left_child->height = 0;
					if (abs(get_height(tmp->left_child) - get_height(tmp->right_child)) > 1)
					{
						//balance
					}
					tmp = tmp->left_child;
					
				}
				tmp = tmp->left_child;
				break;
			}
			case 0:
			{
				throw AVL_KeyExistsException("Key already exists. Please, check your data and key and try again.");
			}
			case 1:
			{
				if (!tmp->right_child)
				{
					tmp->right_child = new AVL_Node(key, data);
					tmp = tmp->right_child;
				}
				tmp = tmp->right_child;
				break;
			}
		}
	}
}

template <typename TKey, typename TData>
void AVL_Tree<TKey, TData>::clean_tree(AVL_Node*& node)
{
	if (node != nullptr)
	{
		clean_tree(node->left_child);
		clean_tree(node->right_child);
		delete node;
	}
}

template <typename TKey, typename TData>
void AVL_Tree<TKey, TData>::prefix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const
{
	prefix(call_back, root);
	return;
}


template <typename TKey, typename TData>
void  AVL_Tree<TKey, TData>::infix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const
{
	infix(call_back, root);
	return;
}


template <typename TKey, typename TData>
void AVL_Tree<TKey, TData>::postfix_stepover_tree(void(*call_back)(const TKey&, const TData&, int)) const
{
	postfix(call_back, root);
	return;
}

template <typename TKey, typename TData>
void AVL_Tree<TKey, TData>::prefix(void(*call_back)(const TKey&, const TData&, int), AVL_Node* cur_root, int depth) const
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
void AVL_Tree<TKey, TData>::infix(void(*call_back)(const TKey&, const TData&, int), AVL_Node* cur_root, int depth) const
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
void AVL_Tree<TKey, TData>::postfix(void(*call_back)(const TKey&, const TData&, int), AVL_Node* cur_root, int depth) const
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
#pragma once
#include "BST.h"
template <typename TKey, typename TData>
class Splay_Tree : public ClassicBinaryTree<TKey, TData>
{
private:

	class remove_Splay final : public ClassicBinaryTree<TKey, TData>::remove_BST
	{
		TData remove(typename ClassicBinaryTree<TKey, TData>::Node*& root, comparer<TKey>* comparator, const TKey& key, ClassicBinaryTree<TKey, TData>* const& tree_ptr) override;
	protected:
		void merge(typename ClassicBinaryTree<TKey, TData>::Node*& left_sub_tree, typename ClassicBinaryTree<TKey, TData>::Node*& right_sub_tree, typename ClassicBinaryTree<TKey, TData>::Node*& new_root) const;
	};

	class find_Splay final : public ClassicBinaryTree<TKey, TData>::find_BST
	{
		TData get_data(typename ClassicBinaryTree<TKey, TData>::Node* const& root, comparer<TKey>* comparator, const TKey& key) const override;
	};

	class Make_Rebalance_Splay final : public ClassicBinaryTree<TKey, TData>::Make_Rebalance
	{
	public:
		void splay(typename ClassicBinaryTree<TKey, TData>::Node**& new_root_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const;
	protected:
		void making_balance(typename ClassicBinaryTree<TKey, TData>::Node**& new_root_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const override;
		void clearing(std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way, typename ClassicBinaryTree<TKey, TData>::Node**& new_root) const override;
		void zig(typename ClassicBinaryTree<TKey, TData>::Node**& new_node, typename ClassicBinaryTree<TKey, TData>::Node**& parent) const;
		void zig_zig(typename ClassicBinaryTree<TKey, TData>::Node**& new_node, typename ClassicBinaryTree<TKey, TData>::Node**& parent, typename ClassicBinaryTree<TKey, TData>::Node**& grand_parent) const;
		void zig_zag(typename ClassicBinaryTree<TKey, TData>::Node**& new_node, typename ClassicBinaryTree<TKey, TData>::Node**& parent, typename ClassicBinaryTree<TKey, TData>::Node**& grand_parent) const;
	};


public:
	Splay_Tree(comparer<TKey>* cmp)
		: ClassicBinaryTree<TKey, TData>(cmp, new typename ClassicBinaryTree<TKey, TData>::add_BST, new remove_Splay, new find_Splay, new typename ClassicBinaryTree<TKey, TData>::rotation_template, new Make_Rebalance_Splay)
	{

	}

	Splay_Tree(const Splay_Tree<TKey, TData>& tree)
		: ClassicBinaryTree<TKey, TData>(tree.cmp, new typename ClassicBinaryTree<TKey, TData>::add_BST, new remove_Splay, new find_Splay, new typename ClassicBinaryTree<TKey, TData>::rotation_template, new Make_Rebalance_Splay)
	{
		this->copy_for_equal(tree.root);
	}

};

#pragma region Make_Rebalance_Splay

template <typename TKey, typename TData>
void Splay_Tree<TKey, TData>::Make_Rebalance_Splay::clearing(std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way, typename ClassicBinaryTree<TKey, TData>::Node**& new_root) const
{

}

template <typename TKey, typename TData> //SPLAY FUNCTION
void Splay_Tree<TKey, TData>::Make_Rebalance_Splay::making_balance(typename ClassicBinaryTree<TKey, TData>::Node**& new_root_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const
{
	splay(new_root_node, way);
}

template <typename TKey, typename TData> //SPLAY FUNCTION
void Splay_Tree<TKey, TData>::Make_Rebalance_Splay::splay(typename ClassicBinaryTree<TKey, TData>::Node**& new_root_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const
{
	typename ClassicBinaryTree<TKey, TData>::Node** new_node = nullptr;
	typename ClassicBinaryTree<TKey, TData>::Node** parent_node = nullptr;
	typename ClassicBinaryTree<TKey, TData>::Node** grand_parent_node = nullptr;
	new_node = way.top();
	way.pop();

	while (!way.empty())
	{
		parent_node = way.top();
		way.pop();
		if (way.empty())
		{
			zig(new_node, parent_node);
			break;
		}
		grand_parent_node = way.top();
		way.pop();

		if ((*grand_parent_node)->left_child == *parent_node)
		{

			if ((*parent_node)->left_child == *new_node)
			{
				zig_zig(new_node, parent_node, grand_parent_node);
				continue;
			}
			else
			{
				zig_zag(new_node, parent_node, grand_parent_node);
				continue;
			}
		}
		else 
		{
			if ((*parent_node)->right_child == *new_node)
			{
				zig_zig(new_node, parent_node, grand_parent_node);
				continue;
			}
			else
			{
				zig_zag(new_node, parent_node, grand_parent_node);
				continue;
			}

		}
	}
	new_root_node = new_node;
}

template <typename TKey, typename TData>
void Splay_Tree<TKey, TData>::Make_Rebalance_Splay::zig(typename ClassicBinaryTree<TKey, TData>::Node**& new_node, typename ClassicBinaryTree<TKey, TData>::Node**& parent) const
{
	typename ClassicBinaryTree<TKey, TData>::rotation_template rotator;
	if ((*parent)->left_child == *new_node)
	{
		rotator.right_rotation(*parent);
		new_node = parent;
	}
	else if ((*parent)->right_child == *new_node)
	{
		rotator.left_rotation(*parent);
		new_node = parent;
	}
}

template <typename TKey, typename TData>
void Splay_Tree<TKey, TData>::Make_Rebalance_Splay::zig_zig(typename ClassicBinaryTree<TKey, TData>::Node**& new_node, typename ClassicBinaryTree<TKey, TData>::Node**& parent, typename ClassicBinaryTree<TKey, TData>::Node **&grand_parent) const
{
	typename ClassicBinaryTree<TKey, TData>::rotation_template rotator;
	if ((*grand_parent)->left_child == *parent)
	{
		rotator.right_rotation(*parent);
		new_node = parent;
		rotator.right_rotation(*grand_parent);
		new_node = grand_parent;
	}
	else if ((*grand_parent)->right_child == *parent)
	{
		rotator.left_rotation(*parent);
		new_node = parent;
		rotator.left_rotation(*grand_parent);
		new_node = grand_parent;
	}
}

template <typename TKey, typename TData>
void Splay_Tree<TKey, TData>::Make_Rebalance_Splay::zig_zag(typename ClassicBinaryTree<TKey, TData>::Node**& new_node, typename ClassicBinaryTree<TKey, TData>::Node**& parent, typename ClassicBinaryTree<TKey, TData>::Node**& grand_parent) const
{
	typename ClassicBinaryTree<TKey, TData>::rotation_template rotator;
	if ((*grand_parent)->left_child == *parent)
	{
		rotator.left_rotation(*parent);
		new_node = parent;
		rotator.right_rotation(*grand_parent);
		new_node = grand_parent;
	}
	else if ((*grand_parent)->right_child == *parent)
	{
		rotator.right_rotation(*parent);
		new_node = parent;
		rotator.left_rotation(*grand_parent);
		new_node = grand_parent;
	}
}

#pragma endregion

#pragma region Find_splay

template <typename TKey, typename TData>
TData Splay_Tree<TKey, TData>::find_Splay::get_data(typename ClassicBinaryTree<TKey, TData>::Node* const& root, comparer<TKey>* comparator, const TKey& key) const
{
	TData rem_data;
	remove_Splay remover;
	std::stack<typename ClassicBinaryTree<TKey, TData>::Node**> stack_way;
	typename ClassicBinaryTree<TKey, TData>::Node** not_const_root;
	Make_Rebalance_Splay rebalancer;
	typename ClassicBinaryTree<TKey, TData>::Node** new_root_node = nullptr;


	not_const_root = const_cast<typename ClassicBinaryTree<TKey, TData>::Node**>(&root);

	stack_way = remover.find_way(*not_const_root, comparator, key);
	rebalancer.splay(new_root_node, stack_way);
	*not_const_root = *new_root_node;
	rem_data = root->data;
	return rem_data;
}
#pragma endregion

#pragma region Remove_splay
template <typename TKey, typename TData>
TData Splay_Tree<TKey, TData>::remove_Splay::remove(typename ClassicBinaryTree<TKey, TData>::Node*& root, comparer<TKey>* comparator, const TKey& key, ClassicBinaryTree<TKey, TData>* const& tree_ptr)
{
	TData rem_data;
	std::stack<typename ClassicBinaryTree<TKey, TData>::Node**> stack_way;
	remove_Splay remover;
	Make_Rebalance_Splay rebalancer;
	stack_way = remover.find_way(root, comparator, key);

	typename ClassicBinaryTree<TKey, TData>::Node** node_for_remove = stack_way.top();

	typename ClassicBinaryTree<TKey, TData>::Node* ptr_on_remove_node = &(*(*node_for_remove));
	rebalancer.splay(node_for_remove, stack_way);
	

	typename ClassicBinaryTree<TKey, TData>::Node* left_subtree;
	left_subtree = &(*(ptr_on_remove_node->left_child));
	typename ClassicBinaryTree<TKey, TData>::Node* right_subtree;
	right_subtree = &(*(ptr_on_remove_node->right_child));
	

	merge(left_subtree, right_subtree, root);
	ptr_on_remove_node->right_child = nullptr;
	ptr_on_remove_node->left_child = nullptr;
	rem_data = ptr_on_remove_node->data;
	delete ptr_on_remove_node;
	return rem_data;
}

template <typename TKey, typename TData>
void Splay_Tree<TKey, TData>::remove_Splay::merge(typename ClassicBinaryTree<TKey, TData>::Node*& left_sub_tree, typename ClassicBinaryTree<TKey, TData>::Node*& right_sub_tree, typename ClassicBinaryTree<TKey, TData>::Node*& new_root) const
{
	typename ClassicBinaryTree<TKey, TData>::Node** tmp_ptr = &left_sub_tree;
	if (*tmp_ptr == nullptr)
	{
		new_root = right_sub_tree;
		return;
	}
	std::stack<typename ClassicBinaryTree<TKey, TData>::Node**> stack_way;
	while (*tmp_ptr != nullptr)
	{
		stack_way.push(tmp_ptr);
		tmp_ptr = &(*tmp_ptr)->right_child;
	}
	Make_Rebalance_Splay rebalancer;
	typename ClassicBinaryTree<TKey, TData>::Node** new_root_node = stack_way.top();
	rebalancer.splay(new_root_node, stack_way);
	(*new_root_node)->right_child = right_sub_tree;
	new_root = *new_root_node;
}
#pragma endregion
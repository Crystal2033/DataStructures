#pragma once

#include "BST.h"

template <typename TKey, typename TData>
class AVL_Tree final : public ClassicBinaryTree<TKey, TData>
{
private:
	class AVL_Node final : public ClassicBinaryTree<TKey, TData>::Node
	{
	public:
		int height = 0;
	public:
		void invalidate_height();
	public:
		AVL_Node();
		AVL_Node(const TKey& key, const TData& data);
	};

private:

	class Make_Rebalance_AVL final : public ClassicBinaryTree<TKey, TData>::Make_Rebalance
	{
	protected:
		void making_balance(typename ClassicBinaryTree<TKey, TData>::Node**& tmp_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const override;
	};

	class add_AVL final : public ClassicBinaryTree<TKey, TData>::add_BST
	{
	protected:
		void allocate_node(typename ClassicBinaryTree<TKey, TData>::Node** to_allocate, comparer<TKey>* const& comparator, const TKey& key, const TData& data, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const override;
	};

	class remove_AVL final : public ClassicBinaryTree<TKey, TData>::remove_BST
	{
	public:
		void after_delete_balance(std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way, typename ClassicBinaryTree<TKey, TData>::Node*& root) const override;
	};

	class rotation_template_AVL final : public ClassicBinaryTree<TKey, TData>::rotation_template
	{
	public:
		void balance(AVL_Node*& bad_node) const;
	protected:
		void after_left_rotation(typename ClassicBinaryTree<TKey, TData>::Node* new_root) const override;
		void after_right_rotation(typename ClassicBinaryTree<TKey, TData>::Node* new_root) const override;
	};

public:
	AVL_Tree(comparer<TKey>* cmp)
		: ClassicBinaryTree<TKey, TData>(cmp, new add_AVL, new remove_AVL, new typename ClassicBinaryTree<TKey, TData>::find_BST, new rotation_template_AVL, new Make_Rebalance_AVL)
	{

	}

	AVL_Tree(const AVL_Tree<TKey, TData>& tree)
		: ClassicBinaryTree<TKey, TData>(tree.cmp, new add_AVL, new remove_AVL, new typename ClassicBinaryTree<TKey, TData>::find_BST, new rotation_template_AVL, new Make_Rebalance_AVL)
	{
		this->copy_for_equal(tree.root);
	}

};

#pragma region AVL_Node

template<typename TKey, typename TData>
AVL_Tree<TKey, TData>::AVL_Node::AVL_Node()
	: ClassicBinaryTree<TKey, TData>::Node(), height(0)
{

}
template<typename TKey, typename TData>
AVL_Tree<TKey, TData>::AVL_Node::AVL_Node(const TKey& key, const TData& data)
	: ClassicBinaryTree<TKey, TData>::Node(key, data), height(0)
{

}

template <typename TKey, typename TData>
void AVL_Tree<TKey, TData>::AVL_Node::invalidate_height()
{
	AVL_Node* left_child_avl = reinterpret_cast<AVL_Node*>(ClassicBinaryTree<TKey, TData>::Node::left_child);
	AVL_Node* right_child_avl = reinterpret_cast<AVL_Node*>(ClassicBinaryTree<TKey, TData>::Node::right_child);

	height = max(
		left_child_avl == nullptr
		    ? -1
		    : left_child_avl->height,
		right_child_avl == nullptr
		    ? -1
		    : right_child_avl->height)
		+ 1;
}

#pragma endregion

#pragma region rotation_template overrides from rotation_template_AVL

template <typename TKey, typename TData>
void AVL_Tree<TKey, TData>::rotation_template_AVL::after_left_rotation(typename ClassicBinaryTree<TKey, TData>::Node* new_root) const
{
	reinterpret_cast<AVL_Node*>(new_root->left_child)->invalidate_height();
	reinterpret_cast<AVL_Node*>(new_root)->invalidate_height();
}

template <typename TKey, typename TData>
void AVL_Tree<TKey, TData>::rotation_template_AVL::after_right_rotation(typename ClassicBinaryTree<TKey, TData>::Node* new_root) const
{
	reinterpret_cast<AVL_Node*>(new_root->right_child)->invalidate_height();
	reinterpret_cast<AVL_Node*>(new_root)->invalidate_height();
}

#pragma endregion

#pragma region add_BST overrides from add_AVL

template <typename TKey, typename TData>
void AVL_Tree<TKey, TData>::add_AVL::allocate_node(typename ClassicBinaryTree<TKey, TData>::Node** to_allocate, comparer<TKey>* const& comparator, const TKey& key, const TData& data, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const
{
	if (*to_allocate == nullptr) //for root
	{
		*to_allocate = new AVL_Node(key, data);
		way.push(to_allocate);
		return;
	}
	if (comparator->compare(key, (*to_allocate)->key) > 0) //Значит, ушел вправо --> добавляем в правое поддерево.
	{
		(*to_allocate)->right_child = new AVL_Node(key, data);
		way.push(&((*to_allocate)->right_child));
	}
	else
	{
		(*to_allocate)->left_child = new AVL_Node(key, data);
		way.push(&((*to_allocate)->left_child));
	}
}


#pragma endregion

#pragma region remove_BST overrides from remove_AVL

template<typename TKey, typename TData>
void AVL_Tree<TKey, TData>::remove_AVL::after_delete_balance(std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way, typename ClassicBinaryTree<TKey, TData>::Node*& root) const
{
	if (!way.empty())
	{
		this->rebalance_ptr->rebalance(way);
	}
}


#pragma endregion

#pragma region miscellaneous methods

template <typename TKey, typename TData>
void AVL_Tree<TKey, TData>::rotation_template_AVL::balance(typename AVL_Tree<TKey, TData>::AVL_Node*& bad_node) const
{
	typename AVL_Tree<TKey, TData>::AVL_Node* bad_node_right_ch = reinterpret_cast<typename AVL_Tree<TKey, TData>::AVL_Node*>(bad_node->right_child);
	typename AVL_Tree<TKey, TData>::AVL_Node* bad_node_left_ch = reinterpret_cast<typename AVL_Tree<TKey, TData>::AVL_Node*>(bad_node->left_child);
	
	int right_height = bad_node_right_ch == nullptr ? -1 : bad_node_right_ch->height;
	int left_height = bad_node_left_ch == nullptr ? -1 : bad_node_left_ch->height;

	int balance_factor = right_height - left_height;

	rotation_template_AVL object_rotate_template;
	typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(bad_node);
	switch (balance_factor)
	{
	case -2:
		object_rotate_template.right_rotation(bad_node_base);
		bad_node = reinterpret_cast<AVL_Node*>(bad_node_base);
		break;
	case 2:
		object_rotate_template.left_rotation(bad_node_base);
		bad_node = reinterpret_cast<AVL_Node*>(bad_node_base);
		break;
	}
}

#pragma endregion
 
#pragma region Rebalance_Mthods_AVL
template<typename TKey, typename TData>
void AVL_Tree<TKey, TData>::Make_Rebalance_AVL::making_balance(typename ClassicBinaryTree<TKey, TData>::Node**& tmp_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& stack_way) const
{
	AVL_Node** temp_tmp_node = reinterpret_cast<AVL_Node**&>(tmp_node);
	temp_tmp_node = reinterpret_cast<AVL_Node**>(stack_way.top());
	if (*temp_tmp_node == nullptr)
	{
		return;
	}
	AVL_Node* tmp_node_right_ch = reinterpret_cast<AVL_Node*>((*temp_tmp_node)->right_child);
	AVL_Node* tmp_node_left_ch = reinterpret_cast<AVL_Node*>((*temp_tmp_node)->left_child);
	(*temp_tmp_node)->invalidate_height();
	int right_height = tmp_node_right_ch == nullptr ? -1 : tmp_node_right_ch->height;
	int left_height = tmp_node_left_ch == nullptr ? -1 : tmp_node_left_ch->height;

	if (abs(right_height - left_height) > 1)
	{
		rotation_template_AVL object_rot_template;
		object_rot_template.balance(*temp_tmp_node);
		typename ClassicBinaryTree<TKey, TData>::Node** new_node_ptr = stack_way.top();
		new_node_ptr = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node**>(tmp_node);
	}
	tmp_node = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node**&>(temp_tmp_node);
}

#pragma endregion
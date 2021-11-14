#pragma once
#include "BST.h"
enum COLOR{RED, BLACK};
template<typename TKey, typename TData>
class RB_Tree final : public ClassicBinaryTree<TKey, TData>
{
private:
	class RB_Node final : public ClassicBinaryTree<TKey, TData>::Node
	{
	public:
		COLOR color;
		RB_Node();
		RB_Node(const TKey& key, const TData& data);
		RB_Node(const TKey& key, const TData& data, const COLOR& color);
	};


	class add_RB final : public ClassicBinaryTree<TKey, TData>::add_BST
	{
	//we have
	/*public:
		virtual void add(Node*& root, comparer<TKey>* comparator, const TKey& key, const TData& data, ClassicBinaryTree<TKey, TData>* const& tree_ptr);*/
	protected:
		void allocate_node(typename ClassicBinaryTree<TKey, TData>::Node** to_allocate, comparer<TKey>* const& comparator, const TKey& key, const TData& data, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const override;
	};


	class remove_RB final : public ClassicBinaryTree<TKey, TData>::remove_BST
	{
	private:
		bool rebalance_without_delete = false;
	//we have
	/*public:
		TData remove(Node*& root, comparer<TKey>* comparator, const TKey& key, ClassicBinaryTree<TKey, TData>* const& tree_ptr);*/
	protected:
		void after_delete_balance(std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way, typename ClassicBinaryTree<TKey, TData>::Node*& root) const override;

		TData remove_leaf(typename ClassicBinaryTree<TKey, TData>::Node*& remove_node, typename ClassicBinaryTree<TKey, TData>::Node*& parent, const TKey& key, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const override;
		TData remove_without_right_branch(typename ClassicBinaryTree<TKey, TData>::Node*& remove_node, typename ClassicBinaryTree<TKey, TData>::Node*& parent, const TKey& key, typename ClassicBinaryTree<TKey, TData>::Node*& root) const override;
		TData remove_without_left_branch(typename ClassicBinaryTree<TKey, TData>::Node*& remove_node, typename ClassicBinaryTree<TKey, TData>::Node*& parent, const TKey& key, typename ClassicBinaryTree<TKey, TData>::Node*& root) const override;
		void refresh_stack_data(typename ClassicBinaryTree<TKey, TData>::Node*& parent, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const override;
		
		
		TData recolour_and_del_after_rot(RB_Node**& parent_rem_node, RB_Node**& remove_node) const;
		void set_black_color_rb(typename ClassicBinaryTree<TKey, TData>::Node*& parent) const;


		TData red_parent_cases(RB_Node**& remove_node, RB_Node**& parent, const TKey& key) const;
		TData black_parent_cases(RB_Node**& remove_node, RB_Node**& parent, const TKey& key, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const;

		TData black_brother_black_father(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, const TKey& key, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const;
		TData red_brother_black_father(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, const TKey& key) const;

		TData red_brother_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node) const;
		TData red_brother_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node) const;

		TData red_brother_kid_right_or_left_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const;
		TData red_brother_kid_right_or_left_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const;

		TData black_parent_black_brother_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const;
		TData black_parent_black_brother_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const;

		TData red_brother_kid_right_has_nephews_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid, RB_Node**& kid_brother_kid) const;
		TData red_brother_kid_right_no_nephews_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const;

		TData red_brother_kid_left_has_nephews_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid, RB_Node**& kid_brother_kid) const;
		TData red_brother_kid_left_no_nephews_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const;

		
		TData black_parent_black_brother_red_right_nephew_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const;
		TData black_parent_black_brother_red_left_nephew_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const;

		TData black_parent_black_brother_red_left_nephew_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const;
		TData black_parent_black_brother_red_right_nephew_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const;

		TData brother_kid_right_rem_left_no_red(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const;
		TData brother_kid_right_rem_left_some_red(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid, RB_Node**& kid_brother_kid) const;

		/*TData black_parent_black_brother_no_nephews_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const;
		TData black_parent_black_brother_no_nephews_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const;*/
		TData black_parent_black_brother_no_nephews(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const;

	};
	

	class rotation_template_RB final : public ClassicBinaryTree<TKey, TData>:: rotation_template
	{
	// we have rotations
	//public:
	//	void left_rotation(ClassicBinaryTree::Node*& old_root) const; //Одинаковы для всех деревьев.
	//	void right_rotation(ClassicBinaryTree::Node*& old_root) const;
	protected:
		void after_left_rotation(typename ClassicBinaryTree<TKey, TData>::Node* new_root) const override;
		void after_right_rotation(typename ClassicBinaryTree<TKey, TData>::Node* new_root) const override;
	};


	class Make_Rebalance_RB final : public ClassicBinaryTree<TKey, TData>::Make_Rebalance
	{
	//we have 
	/*public:
		Node* rebalance(std::stack<Node**>& way) const;*/
	protected:
		void making_balance(typename ClassicBinaryTree<TKey, TData>::Node**& tmp_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const override;
		void clearing(std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way, typename ClassicBinaryTree<TKey, TData>::Node**& new_root) const;
		RB_Node** get_ptr_from_stack(std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const;
		void red_uncle_case_add(RB_Node**&, RB_Node**&, RB_Node**&, RB_Node**&, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>&) const;
		void black_uncle_case_add_gr_par_left(RB_Node**&, RB_Node**&, RB_Node**&, typename ClassicBinaryTree<TKey, TData>::Node**& tmp_node) const;
		void black_uncle_case_add_gr_par_right(RB_Node**&, RB_Node**&, RB_Node**&, typename ClassicBinaryTree<TKey, TData>::Node**& tmp_node) const;

	};


public:
	RB_Tree(comparer<TKey>* cmp)
		: ClassicBinaryTree<TKey, TData>(cmp, new add_RB, new remove_RB, new typename ClassicBinaryTree<TKey, TData>::find_BST, new rotation_template_RB, new Make_Rebalance_RB)
	{

	}

	RB_Tree(const RB_Tree<TKey, TData>& tree)
		: ClassicBinaryTree<TKey, TData>(tree.cmp, new add_RB, new remove_RB, new typename ClassicBinaryTree<TKey, TData>::find_BST, new rotation_template_RB, new Make_Rebalance_RB)
	{
		this->copy_for_equal(tree.root);
	}
};

#pragma region RB_Node methods
template<typename TKey, typename TData>
RB_Tree<TKey, TData>::RB_Node::RB_Node()
	: ClassicBinaryTree<TKey, TData>::Node(), color(RED)
{

}

template<typename TKey, typename TData>
RB_Tree<TKey, TData>::RB_Node::RB_Node(const TKey& key, const TData& data)
	: ClassicBinaryTree<TKey, TData>::Node(key, data), color(RED)
{

}

template<typename TKey, typename TData>
RB_Tree<TKey, TData>::RB_Node::RB_Node(const TKey& key, const TData& data, const COLOR& new_color)
	: ClassicBinaryTree<TKey, TData>::Node(key, data), color(new_color)
{

}

#pragma endregion

#pragma region add_RB implemetation
template<typename TKey, typename TData>
void RB_Tree<TKey, TData>::add_RB::allocate_node(typename ClassicBinaryTree<TKey, TData>::Node** to_allocate, comparer<TKey>* const& comparator, const TKey& key, const TData& data, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const
{
	if (*to_allocate == nullptr) //for root
	{
		*to_allocate = new RB_Node(key, data, BLACK);
		way.push(to_allocate);
		return;
	}
	if (comparator->compare(key, (*to_allocate)->key) > 0) //Значит, ушел вправо --> добавляем в правое поддерево.
	{
		(*to_allocate)->right_child = new RB_Node(key, data);
		way.push(&((*to_allocate)->right_child));
	}
	else
	{
		(*to_allocate)->left_child = new RB_Node(key, data);
		way.push(&((*to_allocate)->left_child));
	}
}

#pragma endregion

#pragma region Make_Rebalance_RB implementation
template<typename TKey, typename TData>
void RB_Tree<TKey, TData>::Make_Rebalance_RB::red_uncle_case_add(RB_Node**& parent_z, RB_Node**& uncle_z, RB_Node**& grandparent_z, RB_Node**& z_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& copy_stack_way) const
{
	(*parent_z)->color = BLACK;
	(*uncle_z)->color = BLACK;
	(*grandparent_z)->color = RED;

	z_node = grandparent_z;
	parent_z = get_ptr_from_stack(copy_stack_way);
}

template<typename TKey, typename TData>
void RB_Tree<TKey, TData>::Make_Rebalance_RB::black_uncle_case_add_gr_par_left(RB_Node**& parent_z, RB_Node**& grandparent_z, RB_Node**& z_node, typename ClassicBinaryTree<TKey, TData>::Node**& tmp_node) const
{
	rotation_template_RB rotation_object;
	typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = nullptr;
	if (*z_node == (*parent_z)->right_child)
	{
		bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent_z);
		rotation_object.left_rotation(bad_node_base);
		*parent_z = reinterpret_cast<RB_Node*>(bad_node_base);
		z_node = reinterpret_cast<RB_Node**>(&((*parent_z)->left_child));
	}

	(*parent_z)->color = BLACK;
	(*grandparent_z)->color = RED;
	bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*grandparent_z);
	rotation_object.right_rotation(bad_node_base);
	*grandparent_z = reinterpret_cast<RB_Node*>(bad_node_base);
	parent_z = grandparent_z;
	tmp_node = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node**&>(parent_z);
}

template<typename TKey, typename TData>
void RB_Tree<TKey, TData>::Make_Rebalance_RB::black_uncle_case_add_gr_par_right(RB_Node**& parent_z, RB_Node**& grandparent_z, RB_Node**& z_node, typename ClassicBinaryTree<TKey, TData>::Node**& tmp_node) const
{
	rotation_template_RB rotation_object;
	typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = nullptr;
	if (*z_node == (*parent_z)->left_child)
	{
		bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent_z);
		rotation_object.right_rotation(bad_node_base);
		*parent_z = reinterpret_cast<RB_Node*>(bad_node_base);
		z_node = reinterpret_cast<RB_Node**>(&((*parent_z)->right_child));

	}
	(*parent_z)->color = BLACK;
	(*grandparent_z)->color = RED;
	bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*grandparent_z);
	rotation_object.left_rotation(bad_node_base);
	*grandparent_z = reinterpret_cast<RB_Node*>(bad_node_base);
	parent_z = grandparent_z;
	tmp_node = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node**&>(parent_z);
}


template<typename TKey, typename TData>
void RB_Tree<TKey, TData>::Make_Rebalance_RB::making_balance(typename ClassicBinaryTree<TKey, TData>::Node**& root_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const
{
	RB_Node** z_node = get_ptr_from_stack(way);
	RB_Node** parent_z = get_ptr_from_stack(way);
	if (parent_z == nullptr)
	{
		return;
	}

	RB_Node** uncle_z = nullptr;
	RB_Node** grandparent_z = nullptr;
	rotation_template_RB rotation_object;

	if ((*parent_z)->color == BLACK)
	{
		root_node = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node**&>(parent_z);
	}


	while ((*parent_z)->color == RED)
	{
		grandparent_z = get_ptr_from_stack(way); //Its not necessary to check out, because parent is red, parent can`t be under red node. But after first loop .p.p could be null.
		if (grandparent_z == nullptr)
		{
			if (way.empty())
			{
				root_node = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node**&>(parent_z);
			}
			break;
		}

		if (*parent_z == (*grandparent_z)->left_child)
		{
			uncle_z = reinterpret_cast<RB_Node**>(&(*grandparent_z)->right_child);
			if (*uncle_z != nullptr)
			{
				if ((*uncle_z)->color == RED)
				{
					red_uncle_case_add(parent_z, uncle_z, grandparent_z, z_node, way);
					if (way.empty() && parent_z == nullptr)
					{
						typename ClassicBinaryTree<TKey, TData>::Node** tmp_z_node = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node**>(z_node);
						way.push(tmp_z_node); //for clearing function to make black root.
					}
					else if (way.empty() && parent_z != nullptr)
					{
						typename ClassicBinaryTree<TKey, TData>::Node** tmp_parent_z_node = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node**>(parent_z);
						way.push(tmp_parent_z_node); //for clearing function to make black root.
					}
					if (parent_z == nullptr) //to save while(*parent_z...)
					{
						break;
					}
				}
				else
				{
					black_uncle_case_add_gr_par_left(parent_z, grandparent_z, z_node, root_node);
				}
			}
			else
			{
				black_uncle_case_add_gr_par_left(parent_z, grandparent_z, z_node, root_node);
			}
			
		}
		else if(*parent_z == (*grandparent_z)->right_child)
		{
			uncle_z = reinterpret_cast<RB_Node**>(&(*grandparent_z)->left_child);
			if (*uncle_z != nullptr)
			{
				if ((*uncle_z)->color == RED)
				{
					red_uncle_case_add(parent_z, uncle_z, grandparent_z, z_node, way);
					if (way.empty() && parent_z == nullptr)
					{
						typename ClassicBinaryTree<TKey, TData>::Node** tmp_z_node = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node**>(z_node);
						way.push(tmp_z_node); //for clearing function to make black root.
					}
					else if (way.empty() && parent_z != nullptr)
					{
						typename ClassicBinaryTree<TKey, TData>::Node** tmp_parent_z_node = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node**>(parent_z);
						way.push(tmp_parent_z_node); //for clearing function to make black root.
					}
					if (parent_z == nullptr)
					{
						break;
					}
				}
				else
				{	
					black_uncle_case_add_gr_par_right(parent_z, grandparent_z, z_node, root_node);

				}
			}
			else //uncle is sentinel - black node
			{
				black_uncle_case_add_gr_par_right(parent_z, grandparent_z, z_node, root_node);

			}
		}
	}
	
	if (root_node != nullptr)
	{
		RB_Node** new_root;
		new_root = reinterpret_cast<RB_Node**>(root_node);
		(*new_root)->color = BLACK;
	}
}

template <typename TKey, typename TData>
void RB_Tree<TKey, TData>::Make_Rebalance_RB::clearing(std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way, typename ClassicBinaryTree<TKey, TData>::Node**& new_root) const
{
	typename ClassicBinaryTree<TKey, TData>::Node** tmp_node_ptr = nullptr;

	while (!way.empty())
	{
		tmp_node_ptr = way.top();
		way.pop();
		new_root = tmp_node_ptr;
		if (way.empty())
		{
			RB_Node** root_rb_node = reinterpret_cast<RB_Node**>(new_root);
			(*root_rb_node)->color = BLACK;
		}
	}
}

template<typename TKey, typename TData>
typename RB_Tree<TKey, TData>::RB_Node** RB_Tree<TKey, TData>::Make_Rebalance_RB::get_ptr_from_stack(std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const
{
	RB_Node** returned_ptr = nullptr;
	if (!way.empty())
	{
		returned_ptr = reinterpret_cast<RB_Node**>(way.top());
		way.pop();
	}
	return returned_ptr;
}

#pragma endregion

#pragma region remove_RB implementation
template<typename TKey, typename TData>
void RB_Tree<TKey, TData>::remove_RB::after_delete_balance(std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way, typename ClassicBinaryTree<TKey, TData>::Node*& root) const
{
	
}

template<typename TKey, typename TData>
void RB_Tree<TKey, TData>::remove_RB::refresh_stack_data(typename ClassicBinaryTree<TKey, TData>::Node*& parent, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const
{

}

template<typename TKey, typename TData>
void RB_Tree<TKey, TData>::remove_RB::set_black_color_rb(typename ClassicBinaryTree<TKey, TData>::Node*& new_root) const
{
	RB_Node* rb_node;
	rb_node = reinterpret_cast<RB_Node*>(new_root);
	rb_node->color = BLACK;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::remove_leaf(typename ClassicBinaryTree<TKey, TData>::Node*& remove_node, typename ClassicBinaryTree<TKey, TData>::Node*& parent, const TKey& key, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const
{
	TData rem_data;
	RB_Node** remove_node_RB = reinterpret_cast<RB_Node**>(&remove_node);
	way.pop();
	RB_Node** parent_remove_node_RB = reinterpret_cast<RB_Node**>(&parent);
	if (parent == nullptr)
	{
		//remove_node->left_child = nullptr;
		rem_data = remove_node->data;
		delete remove_node;
		return rem_data;
	}
	if ((*remove_node_RB)->color == RED)
	{
		if (parent->left_child != nullptr && parent->left_child->key == key)
		{
			parent->left_child = nullptr;
			rem_data = remove_node->data;
			delete remove_node;

		}
		else if (parent->right_child != nullptr && parent->right_child->key == key)
		{
			parent->right_child = nullptr;
			rem_data = remove_node->data;
			delete remove_node;
		}
	}

	else if ((*remove_node_RB)->color == BLACK) //It could be only else.
	{
		if ((*parent_remove_node_RB)->color == RED) //Red parent
		{
			rem_data = red_parent_cases(remove_node_RB, parent_remove_node_RB, key);
		}
		else if ((*parent_remove_node_RB)->color == BLACK) //Black parent
		{
			rem_data = black_parent_cases(remove_node_RB, parent_remove_node_RB, key, way);
		}
	}
	typename ClassicBinaryTree<TKey, TData>::Node** previous_parent = way.top();
	way.pop();
	typename ClassicBinaryTree<TKey, TData>::Node** tmp_ptr;
	if (!way.empty())
	{
		tmp_ptr = way.top();
		if ((*parent_remove_node_RB)->key < (*tmp_ptr)->key)
		{
			(*tmp_ptr)->left_child = *parent_remove_node_RB;
		}
		else
		{
			(*tmp_ptr)->right_child = *parent_remove_node_RB;
		}
	}

	return rem_data;
}

//template<typename TKey, typename TData>
//TData RB_Tree<TKey, TData>::remove_RB::black_parent_black_brother_no_nephews_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way)
//{
//	TData rem_data;
//	(*parent)->right_child = nullptr;
//	rem_data = (*remove_node)->data;
//	delete* remove_node;
//	(*brother_rem_node)->color = RED;
//	rebalance_without_delete = true;
//	remove_leaf(nullptr, parent, 0, way);
//	return rem_data;
//
//}
//
//template<typename TKey, typename TData>
//TData RB_Tree<TKey, TData>::remove_RB::black_parent_black_brother_no_nephews_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way)
//{
//	TData rem_data;
//	(*parent)->left_child = nullptr;
//	rem_data = (*remove_node)->data;
//	delete* remove_node;
//	(*brother_rem_node)->color = RED;
//	rebalance_without_delete = true;
//	RB_Node** new_parent = way.top();
//	way.pop();
//	remove_leaf(nullptr, parent, 0, way);
//	return rem_data;
//}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::black_parent_black_brother_no_nephews(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const
{
	TData rem_data;
	
	if ((*parent)->left_child == *remove_node)
	{
		(*parent)->left_child = nullptr;
	}
	else if ((*parent)->right_child == *remove_node)
	{
		(*parent)->right_child = nullptr;
	}
	rem_data = (*remove_node)->data;
	delete* remove_node;


	(*brother_rem_node)->color = RED;
	//rebalance_without_delete = true;
	/*typename ClassicBinaryTree<TKey, TData>::Node* new_parent = reinterpret_cast<RB_Node*>(*way.top());
	typename ClassicBinaryTree<TKey, TData>::Node* not_remove_node = nullptr;
	way.pop();
	remove_leaf(not_remove_node, new_parent, 0, way);*/

	return rem_data;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::recolour_and_del_after_rot(RB_Node**& parent_rem_node, RB_Node**& remove_node) const
{
	TData rem_data;
	RB_Node** new_parent_kid_right = reinterpret_cast<RB_Node**>(&(*parent_rem_node)->right_child);
	RB_Node** new_parent_kid_left = reinterpret_cast<RB_Node**>(&(*parent_rem_node)->left_child);
	(*new_parent_kid_left)->color = BLACK;
	(*parent_rem_node)->color = RED;
	(*new_parent_kid_right)->color = BLACK;
	//(*brother_kid)->color = RED;
	//(*parent_rem_node)->color = BLACK;
	////(*brother_rem_node)->color = BLACK;
	rem_data = (*remove_node)->data;
	delete* remove_node;
	return rem_data;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::red_parent_cases(RB_Node**& remove_node, RB_Node**& parent, const TKey& key) const
{// brother here can not be red, because parent is already red.
	TData rem_data;
	rotation_template_RB rotator;

	RB_Node** brother_rem_node = reinterpret_cast<RB_Node**>(((*parent)->left_child == *remove_node ? &((*parent)->right_child) : &((*parent)->left_child)));
	RB_Node** parent_rem_node = reinterpret_cast<RB_Node**>(parent); //probably useless tmp
	//case 1: 1.1 - black brother has one red, 1.2 - black brother has no red
	if ((*brother_rem_node)->left_child != nullptr || (*brother_rem_node)->right_child != nullptr) //Brother has kid(-s)
	{//1.1 impementation
		RB_Node** brother_kid;

		if ((*brother_rem_node)->left_child != nullptr && (*brother_rem_node)->right_child != nullptr) // has 2 kids
		{
			RB_Node** brother_second_kid;
			brother_kid = reinterpret_cast<RB_Node**>(&(*brother_rem_node)->left_child);
			brother_second_kid = reinterpret_cast<RB_Node**>(&(*brother_rem_node)->right_child);
			//=============================
			//THEY CAN`T BE BOTH BLACK!!! |
			//=============================
			if ((*parent)->left_child == *brother_rem_node)
			{
				typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent_rem_node);
				(*parent)->right_child = nullptr;
				rotator.right_rotation(bad_node_base);
				*parent_rem_node = reinterpret_cast<RB_Node*>(bad_node_base);

				return recolour_and_del_after_rot(parent_rem_node, remove_node);

			}
			else
			{
				typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent_rem_node);
				(*parent)->left_child = nullptr;
				rotator.left_rotation(bad_node_base);
				*parent_rem_node = reinterpret_cast<RB_Node*>(bad_node_base);

				return recolour_and_del_after_rot(parent_rem_node, remove_node);

			}

		}
		else
		{
			if ((*parent)->left_child == *brother_rem_node)
			{
				if ((*brother_rem_node)->left_child != nullptr)
				{
					//С РАЗНЫХ СТОРОН МОЖЕТ БЫТЬ УДАЛЯЕМЫЙ!!!
					brother_kid = reinterpret_cast<RB_Node**>(&(*brother_rem_node)->left_child); //red brother kid on the LEFT
					typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent_rem_node);
					(*parent)->right_child = nullptr;
					rotator.right_rotation(bad_node_base);
					*parent_rem_node = reinterpret_cast<RB_Node*>(bad_node_base);

					return recolour_and_del_after_rot(parent_rem_node, remove_node);

				}
				else if ((*brother_rem_node)->right_child != nullptr)
				{
					brother_kid = reinterpret_cast<RB_Node**>(&(*brother_rem_node)->right_child); //red brother kid on the RIGHT
					typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*brother_rem_node);
					(*parent)->right_child = nullptr;
					rotator.left_rotation(bad_node_base);
					*brother_rem_node = reinterpret_cast<RB_Node*>(bad_node_base);
					bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent_rem_node);
					rotator.right_rotation(bad_node_base);
					*parent_rem_node = reinterpret_cast<RB_Node*>(bad_node_base);

					return recolour_and_del_after_rot(parent_rem_node, remove_node);
				}
			}
			else
			{
				if ((*brother_rem_node)->right_child != nullptr)
				{
					brother_kid = reinterpret_cast<RB_Node**>(&(*brother_rem_node)->right_child); //red brother kid on the RIGHT
					typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent_rem_node);
					(*parent)->left_child = nullptr;
					rotator.left_rotation(bad_node_base);
					*parent_rem_node = reinterpret_cast<RB_Node*>(bad_node_base);
					return recolour_and_del_after_rot(parent_rem_node, remove_node);

				}
				else if ((*brother_rem_node)->left_child != nullptr)
				{
					brother_kid = reinterpret_cast<RB_Node**>(&(*brother_rem_node)->left_child); //red brother kid on the LEFT
					typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*brother_rem_node);
					(*parent)->left_child = nullptr;
					rotator.right_rotation(bad_node_base);
					*brother_rem_node = reinterpret_cast<RB_Node*>(bad_node_base);
					bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent_rem_node);
					rotator.left_rotation(bad_node_base);
					*parent_rem_node = reinterpret_cast<RB_Node*>(bad_node_base);

					return recolour_and_del_after_rot(parent_rem_node, remove_node);

				}
			}
		}
	}
	else //brother has no kids
	{//1.2 implementation
		(*parent_rem_node)->color = BLACK;
		(*brother_rem_node)->color = RED;
		if ((*parent_rem_node)->left_child == *remove_node)
		{
			(*parent_rem_node)->left_child = nullptr;
			rem_data = (*remove_node)->data;
			delete* remove_node;
		}
		else if ((*parent_rem_node)->right_child == *remove_node)
		{
			(*parent_rem_node)->right_child = nullptr;
			rem_data = (*remove_node)->data;
			delete* remove_node;
		}
	}
	return rem_data;
}



template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::black_parent_cases(RB_Node**& remove_node, RB_Node**& parent, const TKey& key, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const
{
	TData rem_data;
	typename ClassicBinaryTree<TKey, TData>::rotation_template rotator;
	RB_Node** brother_rem_node = reinterpret_cast<RB_Node**>(((*parent)->left_child == *remove_node ? &((*parent)->right_child) : &((*parent)->left_child)));

	if ((*brother_rem_node)->color == RED)
	{
		rem_data = red_brother_black_father(remove_node, parent, brother_rem_node, key);
	}
	else if ((*brother_rem_node)->color == BLACK)
	{
		rem_data = black_brother_black_father(remove_node, parent, brother_rem_node, key, way);
	}
	
	return rem_data; 
}


template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::red_brother_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node) const
{
	RB_Node** brother_kid;
	TData rem_data;
	//РАЗВИЛКА РЕБЕНОК СПРАВА
	if ((*brother_rem_node)->right_child != nullptr)
	{
		brother_kid = reinterpret_cast<RB_Node**>(&(*brother_rem_node)->right_child);
	}
	else if ((*brother_rem_node)->left_child != nullptr)
	{
		brother_kid = reinterpret_cast<RB_Node**>(&(*brother_rem_node)->left_child);
	}

	rem_data = red_brother_kid_right_or_left_rem_right(remove_node, parent, brother_rem_node, brother_kid);

	return rem_data;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::red_brother_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node) const
{
	RB_Node** brother_kid;
	TData rem_data;
	//РАЗВИЛКА РЕБЕНОК СПРАВА
	if ((*brother_rem_node)->right_child != nullptr)
	{
		brother_kid = reinterpret_cast<RB_Node**>(&(*brother_rem_node)->right_child);
	}
	else if ((*brother_rem_node)->left_child != nullptr)
	{
		brother_kid = reinterpret_cast<RB_Node**>(&(*brother_rem_node)->left_child);
	}

	rem_data = red_brother_kid_right_or_left_rem_left(remove_node, parent, brother_rem_node, brother_kid);

	return rem_data;
}
template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::black_parent_black_brother_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const
{
	RB_Node** brother_kid;
	TData rem_data;
	if ((*brother_rem_node)->left_child != nullptr) //у брата справа есть узел.
	{
		brother_kid = reinterpret_cast<RB_Node**>(&(*brother_rem_node)->left_child);
		if ((*brother_kid)->color == RED) //ребенок брата - красный.
		{
			rem_data = black_parent_black_brother_red_left_nephew_rem_left(remove_node, parent, brother_rem_node, brother_kid);
		}
	}
	else if ((*brother_rem_node)->right_child != nullptr) //у брата слева есть узел.
	{
		rem_data = black_parent_black_brother_red_right_nephew_rem_left(remove_node, parent, brother_rem_node, brother_kid);
	}
	else
	{
		black_parent_black_brother_no_nephews(remove_node, parent, brother_rem_node, way);
		//std::cout << red << "You are MEGARETARD retard, Paul, try again. 1" << white << std::endl;
	}
	return rem_data;
}
template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::black_parent_black_brother_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const
{
	RB_Node** brother_kid;
	TData rem_data;
	if ((*brother_rem_node)->right_child != nullptr) //у брата справа есть узел.
	{
		brother_kid = reinterpret_cast<RB_Node**>(&(*brother_rem_node)->right_child);
		if ((*brother_kid)->color == RED) //ребенок брата - красный.
		{
			rem_data = black_parent_black_brother_red_right_nephew_rem_right(remove_node, parent, brother_rem_node, brother_kid);
		}
	}
	else if ((*brother_rem_node)->left_child != nullptr) //у брата слева есть узел.
	{
		brother_kid = reinterpret_cast<RB_Node**>(&(*brother_rem_node)->left_child);
		rem_data = black_parent_black_brother_red_left_nephew_rem_right(remove_node, parent, brother_rem_node, brother_kid);
	}
	else
	{
		black_parent_black_brother_no_nephews(remove_node, parent, brother_rem_node, way);
		//std::cout << red << "You are MEGARETARD retard, Paul, try again. 1" << white << std::endl;
	}
	return rem_data;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::red_brother_kid_right_no_nephews_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const
{
	TData rem_data;
	(*parent)->right_child = nullptr;
	rem_data = (*remove_node)->data;
	delete* remove_node;
	rotation_template_RB rotator;
	(*brother_kid)->color = RED;
	(*brother_rem_node)->color = BLACK;
	typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent);
	rotator.right_rotation(bad_node_base);
	*parent = reinterpret_cast<RB_Node*>(bad_node_base);
	return rem_data;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::red_brother_kid_left_no_nephews_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const
{
	TData rem_data;
	(*parent)->left_child = nullptr;
	rem_data = (*remove_node)->data;
	delete* remove_node;
	rotation_template_RB rotator;
	(*brother_kid)->color = RED;
	(*brother_rem_node)->color = BLACK;
	typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent);
	rotator.left_rotation(bad_node_base);
	*parent = reinterpret_cast<RB_Node*>(bad_node_base);
	return rem_data;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::brother_kid_right_rem_left_no_red(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const
{
	TData rem_data;
	(*parent)->left_child = nullptr;
	rem_data = (*remove_node)->data;
	delete* remove_node;
	rotation_template_RB rotator;
	(*brother_kid)->color = RED;
	//RB_Node** kid_brother_kid = reinterpret_cast<RB_Node**>();
	typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent);
	rotator.left_rotation(bad_node_base);
	*parent = reinterpret_cast<RB_Node*>(bad_node_base);
	(*parent)->color = BLACK;
	return rem_data;
}


template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::red_brother_kid_right_has_nephews_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid, RB_Node**& nephew) const
{
	TData rem_data;
	(*parent)->right_child = nullptr;
	rem_data = (*remove_node)->data;
	delete* remove_node;

	rotation_template_RB rotator;
	typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base;
	if ((*brother_kid)->right_child != nullptr) //значит, есть красный племянник.
	{
		(*brother_rem_node)->color = BLACK;
	}
	else
	{
		(*nephew)->color = BLACK;
	}
	bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*brother_rem_node);
	rotator.left_rotation(bad_node_base);
	*brother_rem_node = reinterpret_cast<RB_Node*>(bad_node_base);
	bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent);
	rotator.right_rotation(bad_node_base);
	*parent = reinterpret_cast<RB_Node*>(bad_node_base);
	return rem_data;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::red_brother_kid_left_has_nephews_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid, RB_Node**& nephew) const
{
	TData rem_data;
	(*parent)->left_child = nullptr;
	rem_data = (*remove_node)->data;
	delete* remove_node;

	rotation_template_RB rotator;
	typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base;
	if ((*brother_kid)->left_child != nullptr) //значит, есть красный племянник.
	{
		(*brother_rem_node)->color = BLACK;
	}
	else
	{
		(*nephew)->color = BLACK;
	}
	bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*brother_rem_node);
	rotator.right_rotation(bad_node_base);
	*brother_rem_node = reinterpret_cast<RB_Node*>(bad_node_base);
	bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent);
	rotator.left_rotation(bad_node_base);
	*parent = reinterpret_cast<RB_Node*>(bad_node_base);
	return rem_data;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::brother_kid_right_rem_left_some_red(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid, RB_Node**& kid_brother_kid) const
{
	TData rem_data;
	(*parent)->left_child = nullptr;
	rem_data = (*remove_node)->data;
	delete* remove_node;
	rotation_template_RB rotator;
	(*kid_brother_kid)->color = BLACK;
	typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*brother_rem_node);
	rotator.right_rotation(bad_node_base);
	*brother_rem_node = reinterpret_cast<RB_Node*>(bad_node_base);
	bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent);
	rotator.left_rotation(bad_node_base);
	*parent = reinterpret_cast<RB_Node*>(bad_node_base);
	/*RB_Node** new_parent_kid_right = reinterpret_cast<RB_Node**>(&(*parent)->right_child);
	RB_Node** new_parent_kid_left = reinterpret_cast<RB_Node**>(&(*parent)->left_child);
	*/
	//RB_Node** node_for_change_color = reinterpret_cast<RB_Node**>(&(*new_parent_kid_left)->right_child);
	//(*new_parent_kid_left)->color = RED;

	return rem_data;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::red_brother_kid_right_or_left_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const 
{
	TData rem_data;
	if ((*brother_rem_node)->right_child == *brother_kid)
	{
		if ((*brother_kid)->left_child != nullptr || (*brother_kid)->right_child != nullptr) // у ребенка брата есть какие-то потомки
		{
			RB_Node** nephew = nullptr;
			if ((*brother_kid)->left_child != nullptr)
			{
				nephew = reinterpret_cast<RB_Node**>(&(*brother_kid)->left_child); //Он может быть только красный
			}
			else if ((*brother_kid)->right_child != nullptr)
			{
				nephew = reinterpret_cast<RB_Node**>((*brother_kid)->right_child);  //Он может быть только красный
			}
			return red_brother_kid_right_has_nephews_rem_right(remove_node, parent, brother_rem_node, brother_kid, nephew);  //NOPPERS
		}
		else
		{//2.1.2  нет красных потомков у ребенка брата -> оба sentinel
			return red_brother_kid_right_no_nephews_rem_right(remove_node, parent, brother_rem_node, brother_kid);
		}
	}
	else if ((*brother_rem_node)->left_child == *brother_kid)
	{
		rotation_template_RB rotator;
		rem_data = (*remove_node)->data;
		(*parent)->right_child = nullptr;
		delete *remove_node;
		typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent);
		(*brother_rem_node)->color = BLACK;
		(*parent)->color = RED;
		rotator.right_rotation(bad_node_base);
		*parent = reinterpret_cast<RB_Node*>(bad_node_base);
	}
	return rem_data;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::red_brother_kid_right_or_left_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const
{
	TData rem_data;
	if ((*brother_rem_node)->left_child == *brother_kid)
	{
		if ((*brother_kid)->left_child != nullptr || (*brother_kid)->right_child != nullptr) // у ребенка брата есть какие-то потомки
		{
			RB_Node** nephew = nullptr;
			if ((*brother_kid)->left_child != nullptr)
			{
				nephew = reinterpret_cast<RB_Node**>(&(*brother_kid)->left_child); //Он может быть только красный
			}
			else if ((*brother_kid)->right_child != nullptr)
			{
				nephew = reinterpret_cast<RB_Node**>((*brother_kid)->right_child);  //Он может быть только красный
			}
			return red_brother_kid_left_has_nephews_rem_left(remove_node, parent, brother_rem_node, brother_kid, nephew);  //NOPPERS
		}
		else
		{//2.1.2  нет красных потомков у ребенка брата -> оба sentinel
			return red_brother_kid_left_no_nephews_rem_left(remove_node, parent, brother_rem_node, brother_kid);
		}
	}
	else if ((*brother_rem_node)->right_child == *brother_kid)
	{
		rotation_template_RB rotator;
		rem_data = (*remove_node)->data;
		(*parent)->left_child = nullptr;
		delete* remove_node;
		typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent);
		(*brother_rem_node)->color = BLACK;
		(*parent)->color = RED;
		rotator.left_rotation(bad_node_base);
		*parent = reinterpret_cast<RB_Node*>(bad_node_base);
	}
	return rem_data;
}


//template<typename TKey, typename TData>
//TData RB_Tree<TKey, TData>::remove_RB::bl_par_red_br_rem_left_br_kid_black(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const //черный родитель, красный брат, удаляемый справа, какой-то ребенок брата - черный
//{
//	TData rem_data;
//	if ((*brother_rem_node)->left_child == *brother_kid)
//	{
//		if ((*brother_kid)->left_child != nullptr || (*brother_kid)->right_child != nullptr) // у ребенка брата есть какие-то потомки
//		{
//			RB_Node** kid_brother_kid_left = nullptr;
//			RB_Node** kid_brother_kid_right = nullptr;
//			if ((*brother_kid)->left_child != nullptr && (*brother_kid)->right_child != nullptr)
//			{//ЧК3 (HABR) 
//				RB_Node** kid_brother_kid_left = reinterpret_cast<RB_Node**>((*brother_kid)->left_child);
//				RB_Node** kid_brother_kid_right = reinterpret_cast<RB_Node**>((*brother_kid)->right_child);
//				if ((*kid_brother_kid_left)->color != RED && (*kid_brother_kid_right)->color != RED)
//				{
//					return brother_kid_right_rem_left_no_red(remove_node, parent, brother_rem_node, brother_kid);
//				} 
//
//			}
//			if ((*brother_kid)->right_child != nullptr)
//			{
//				RB_Node** kid_brother_kid_right = reinterpret_cast<RB_Node**>(&(*brother_kid)->right_child);
//				if ((*kid_brother_kid_right)->color == RED)
//				{
//					return brother_kid_right_rem_left_some_red(remove_node, parent, brother_rem_node, brother_kid, kid_brother_kid_right);  //NOPPERS
//				}
//			}
//			//else if ((*brother_kid)->right_child != nullptr)
//			//{
//			//	RB_Node** kid_brother_kid_right = reinterpret_cast<RB_Node**>((*brother_kid)->left_child);
//			//	if ((*kid_brother_kid_right)->color == RED)
//			//	{
//			//		return brother_kid_right_rem_right_some_red(remove_node, parent, brother_rem_node, brother_kid, kid_brother_kid_right); //NOPPERS
//			//	}
//			//}
//		}
//		else
//		{//2.1.2  нет красных потомков у ребенка брата -> оба черные
//			return brother_kid_right_rem_left_no_red(remove_node, parent, brother_rem_node, brother_kid);
//		}
//	}
//	else if ((*brother_rem_node)->right_child == *brother_kid)
//	{
//
//	}
//	return rem_data;
//}


template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::black_parent_black_brother_red_right_nephew_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const
{
	TData rem_data;
	rotation_template_RB rotator;
	(*parent)->right_child = nullptr;
	rem_data = (*remove_node)->data;
	delete* remove_node;

	(*brother_kid)->color = BLACK;
	typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*brother_rem_node);
	rotator.left_rotation(bad_node_base);
	*brother_rem_node = reinterpret_cast<RB_Node*>(bad_node_base);
	bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent);
	rotator.right_rotation(bad_node_base);
	*parent = reinterpret_cast<RB_Node*>(bad_node_base);
	return rem_data;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::black_parent_black_brother_red_left_nephew_rem_right(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const
{
	TData rem_data;
	rotation_template_RB rotator;
	(*parent)->right_child = nullptr;
	rem_data = (*remove_node)->data;
	delete* remove_node;

	(*brother_kid)->color = BLACK;
	typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent);
	rotator.right_rotation(bad_node_base);
	*parent = reinterpret_cast<RB_Node*>(bad_node_base);
	return rem_data;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::black_parent_black_brother_red_right_nephew_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const
{
	TData rem_data;
	rotation_template_RB rotator;
	(*parent)->left_child = nullptr;
	rem_data = (*remove_node)->data;
	delete* remove_node;

	(*brother_kid)->color = BLACK;
	typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent);
	rotator.left_rotation(bad_node_base);
	*parent = reinterpret_cast<RB_Node*>(bad_node_base);
	return rem_data;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::black_parent_black_brother_red_left_nephew_rem_left(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, RB_Node**& brother_kid) const
{
	TData rem_data;
	rotation_template_RB rotator;
	(*parent)->left_child = nullptr;
	rem_data = (*remove_node)->data;
	delete* remove_node;

	(*brother_kid)->color = BLACK;
	typename ClassicBinaryTree<TKey, TData>::Node* bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*brother_rem_node);
	rotator.right_rotation(bad_node_base);
	*brother_rem_node = reinterpret_cast<RB_Node*>(bad_node_base);
	bad_node_base = reinterpret_cast<typename ClassicBinaryTree<TKey, TData>::Node*>(*parent);
	rotator.left_rotation(bad_node_base);
	*parent = reinterpret_cast<RB_Node*>(bad_node_base);
	return rem_data;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::red_brother_black_father(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, const TKey& key) const
{//2.1 black parent and red brother
	TData rem_data;
	//РАЗВИЛКА УДАЛЯЕМЫЙ СЛЕВА ИЛИ СПРАВА.
	if ((*parent)->right_child == *remove_node)
	{	
		return red_brother_rem_right(remove_node, parent, brother_rem_node);
	}
	//==================================================================================================================================================================
	else if ((*parent)->left_child == *remove_node)
	{
		return red_brother_rem_left(remove_node, parent, brother_rem_node);
	}
	return rem_data;
}
 
template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::black_brother_black_father(RB_Node**& remove_node, RB_Node**& parent, RB_Node**& brother_rem_node, const TKey& key, std::stack<typename ClassicBinaryTree<TKey, TData>::Node**>& way) const
{//2.2
	TData rem_data;

	if ((*parent)->right_child == *remove_node)
	{
		return black_parent_black_brother_rem_right(remove_node, parent, brother_rem_node, way);
	}
	//==================================================================================================================================================================
	else if ((*parent)->left_child == *remove_node)
	{
		return black_parent_black_brother_rem_left(remove_node, parent, brother_rem_node, way); //BLACK_BR + WAY
	}
	return rem_data;
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::remove_without_right_branch(typename ClassicBinaryTree<TKey, TData>::Node*& remove_node, typename ClassicBinaryTree<TKey, TData>::Node*& parent, const TKey& key, typename ClassicBinaryTree<TKey, TData>::Node*& root) const
{
	TData rem_data;
	RB_Node** remove_node_RB = reinterpret_cast<RB_Node**>(&remove_node);
	RB_Node** parent_remove_node_RB = reinterpret_cast<RB_Node**>(&parent);
	RB_Node** child_remove_node = reinterpret_cast<RB_Node**>(&(remove_node->left_child));
	if (parent == nullptr)
	{
		if (remove_node->left_child == nullptr || remove_node->right_child == nullptr)
		{
			root = remove_node->left_child;
			set_black_color_rb(remove_node->left_child);
			remove_node->left_child = nullptr;
			rem_data = remove_node->data;
			delete remove_node;
			return rem_data;
		}
	}

	if (parent->left_child == remove_node)
	{
		parent->left_child = remove_node->left_child;
		(*child_remove_node)->color = BLACK;
		remove_node->left_child = nullptr;

		rem_data = remove_node->data;
		delete remove_node;
		return rem_data;
	}
	else if (parent->right_child == remove_node)
	{
		parent->right_child = remove_node->left_child;
		(*child_remove_node)->color = BLACK;
		remove_node->left_child = nullptr;

		rem_data = remove_node->data;
		delete remove_node;
		return rem_data;
	}
}

template<typename TKey, typename TData>
TData RB_Tree<TKey, TData>::remove_RB::remove_without_left_branch(typename ClassicBinaryTree<TKey, TData>::Node*& remove_node, typename ClassicBinaryTree<TKey, TData>::Node*& parent, const TKey& key, typename ClassicBinaryTree<TKey, TData>::Node*& root) const
{
	TData rem_data;
	RB_Node** remove_node_RB = reinterpret_cast<RB_Node**>(&remove_node);
	RB_Node** parent_remove_node_RB = reinterpret_cast<RB_Node**>(&parent);
	RB_Node** child_remove_node = reinterpret_cast<RB_Node**>(&(remove_node->right_child));
	if (parent == nullptr)
	{
		if (remove_node->left_child == nullptr || remove_node->right_child == nullptr)
		{
			root = remove_node->right_child;
			set_black_color_rb(remove_node->right_child);
			remove_node->left_child = nullptr;
			rem_data = remove_node->data;
			delete remove_node;
			return rem_data;
		}
	}
	if (parent->left_child == remove_node)
	{
		parent->left_child = remove_node->right_child;
		(*child_remove_node)->color = BLACK;
		remove_node->right_child = nullptr;

		rem_data = remove_node->data;
		delete remove_node;
		return rem_data;
	}
	else if (parent->right_child == remove_node)
	{
		parent->right_child = remove_node->right_child;
		(*child_remove_node)->color = BLACK;
		remove_node->right_child = nullptr;

		rem_data = remove_node->data;
		delete remove_node;
		return rem_data;
	}
}
#pragma endregion


#pragma region rotation_template_RB implementation
template<typename TKey, typename TData>
void RB_Tree<TKey, TData>::rotation_template_RB::after_left_rotation(typename ClassicBinaryTree<TKey, TData>::Node* new_root) const
{

}

template<typename TKey, typename TData>
void RB_Tree<TKey, TData>::rotation_template_RB::after_right_rotation(typename ClassicBinaryTree<TKey, TData>::Node* new_root) const
{

}

#pragma endregion
#include "bst.h"
#include <memory>

bst::bst(int number) {
	key = number;
}

bool bst::find(int key) {
	if (this->key == key)
		return true;
	else if (left && this->key > key)
		return left->find(key);
	else if (right && this->key < key)
		return right->find(key);
	return false;
}

// TODO: Think about empty tree insertions
void bst::insert(int key) {
	if (this->key == key)
		throw bst_exception("key already exists");
	std::shared_ptr<bst> node = shared_from_this();
	std::shared_ptr<bst> previous_node;
	while (node) {
		previous_node = node;		
		if (node->key == key)
			throw bst_exception("key already exists");
		if (node->key > key)
			node = node->left;
		else
			node = node->right;
	}
	if (previous_node->key > key) {
		previous_node->left.reset(new bst(key));
		previous_node->left->parent = previous_node;
	} else {
		previous_node->right.reset(new bst(key));
	}
}

std::shared_ptr<bst> search(int key, std::shared_ptr<bst> tree) {
	if (key == tree->key) {
		return tree;
	}
	if (tree->key > key && tree->left) {
		return search(key, tree->left);
	} else if (tree->right) {
		return search(key, tree->right);
	}
	return NULL;
}

void remove_child(int key, std::shared_ptr<bst> tree) {
	if (tree->right && tree->right->key == key) {
		tree->right.reset();
	} else {
		tree->left.reset();
	}
}
void set_child(int key, std::shared_ptr<bst> tree, std::shared_ptr<bst> child) {
	if (tree->left && tree->left->key == key) {
		tree->left = child;
	} else {
		tree->right = child;
	}
}

// Return pointer to most left value
std::shared_ptr<bst> get_minimum(std::shared_ptr<bst> tree) {
	if (tree->left)
		return get_minimum(tree->left);
	return tree;
}

// TODO: Think about deleting last node
void bst::del(int key) {
	if (this->key == key && !left && !right) {
		// Server should handle this
		throw last_node();
	}
	// Search for key
	auto tree = shared_from_this();
	auto branch = search(key, tree);
	if (!branch) {
		throw bst_exception("No key found");
	}
	if (!branch->right && !branch->left) {
		remove_child(key, branch->parent);
		return;
	}
	if (branch->right && !branch->left) {
		if (!branch->parent) {
			*this = *branch->right;
			return;
		}
		set_child(key, branch->parent, branch->right);
	} else if (branch->left && !branch->right) {
		if (!branch->parent) {
			*this = *branch->left;
			return;
		}
		set_child(key, branch->parent, branch->left);
	} else {
		// Both right and left exist
		auto new_branch = get_minimum(branch->right);
		if (new_branch->key != branch->right->key) {
			if (!new_branch->left && !new_branch->right) {
				remove_child(new_branch->key, new_branch->parent);
			} else {
				set_child(new_branch->key, new_branch->parent, new_branch->right);
			}
		} else {
			remove_child(new_branch->key, new_branch->parent);
		}
		new_branch->left = branch->left;
		if (branch->parent) {
			set_child(key, branch->parent, new_branch);
			new_branch->parent = branch->parent;
		} else {
			new_branch->right = tree->right;
			*this = *new_branch;
		}
	}
}


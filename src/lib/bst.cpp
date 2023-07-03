#include "bst.h"
#include <memory>

bst::bst(int number) {
	key = number;
}

bool bst::find(int key) {
	if (this->key == key)
		return true;
	else if (left != NULL && this->key > key)
		return left->find(key);
	else if (right != NULL && this->key < key)
		return right->find(key);
	return false;
}

// TODO: Think about empty tree insertions
void bst::insert(int key) {
	if (this->key == key)
		throw bst_exception("key already exists");
	bst *node = this;
	bst *previous_node;
	while (node != NULL) {
		previous_node = node;
		if (node->key == key)
			throw bst_exception("key already exists");
		if (node->right != NULL && node->key > key)
			node = node->left;
		else
			node = node->right;
	}
	if (previous_node->key > key) {
		previous_node->left = new bst(key);
		previous_node->left->parent = previous_node;
	} else {
		previous_node->right = new bst(key);
		previous_node->right->parent = previous_node;
	}
}

bst* search(int key, bst* tree) {
	if (key == tree->key) {
		return tree;
	}
	if (tree->key > key && tree->left != NULL) {
		return search(key, tree->left);
	} else if (tree->right != NULL) {
		return search(key, tree->right);
	}
	return NULL;
}

void remove_child(int key, bst *tree) {
	if (tree->right != NULL && tree->right->key == key) {
		delete tree->right;
		tree->right = NULL;
	} else {
		delete tree->left;
		tree->left = NULL;
	}
}

void set_child(int key, bst *tree, bst *child) {
	if (tree->left != NULL && tree->left->key == key) {
		tree->left = child;
	} else {
		tree->right = child;
	}

	if (child != NULL)
		child->parent = tree;
}

void null_child(int key, bst *tree) {
	if (tree->right != NULL && tree->right->key == key) {
		tree->right = NULL;
	} else {
		tree->left = NULL;
	}
}


// Return pointer to most left value
bst* get_minimum(bst *tree) {
	if (tree->left != NULL)
		return get_minimum(tree->left);
	return tree;
}

// TODO: Think about deleting last node
void bst::del(int key) {
	if (this->key == key && left == NULL && right == NULL) {
		// Server should handle this
		throw last_node();
	}
	// Search for key
	auto tree = this;
	auto branch = search(key, tree);
	if (branch == NULL) {
		throw bst_exception("No key found");
	}
	if (branch->right == NULL && branch->left == NULL) {
		remove_child(key, branch->parent);
		return;
	}
	if (branch->right && branch->left == NULL) {
		if (branch->parent == NULL) {
			*this = *branch->right;
			return;
		}
		set_child(key, branch->parent, branch->right);
	} else if (branch->left && branch->right == NULL) {
		if (!branch->parent) {
			*this = *branch->left;
			return;
		}
		set_child(key, branch->parent, branch->left);
	} else {
		// Both right and left exist
		auto new_branch = get_minimum(branch->right);
		if (new_branch->key != branch->right->key) {
			if (new_branch->left == NULL && new_branch->right != NULL) {
				null_child(new_branch->key, new_branch->parent);
			} else {
				set_child(new_branch->key, new_branch->parent, new_branch->right);
			}
		}

		new_branch->left = branch->left;
		if (branch->left != NULL) {
			new_branch->left->parent = new_branch;
		}

		if (branch->parent != NULL) {
			set_child(key, branch->parent, new_branch);
			new_branch->parent = branch->parent;
		} else {
			new_branch->right = tree->right;
			new_branch->parent = NULL;
			*this = *new_branch;
		}
	}
}


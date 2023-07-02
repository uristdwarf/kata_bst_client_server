#include "bst.h"

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
	bst *node = this;
	bst *previous_node;
	while (node != NULL) {
		previous_node = node;		
		if (node->key == key)
			throw bst_exception("key already exists");
		if (node->key > key)
			node = node->left.get();
		else
			node = node->right.get();
	}
	if (previous_node->key > key)
		previous_node->left.reset(new bst(key));
	else 
		previous_node->right.reset(new bst(key));
}

// TODO: Think about deleting last node
void bst::del(int key) {
	if (this->key == key && !left && !right) {
		// Server should handle this
		throw last_node();
	}
}

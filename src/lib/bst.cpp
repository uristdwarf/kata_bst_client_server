#include "bst.h"

bool bst::find(int key) {
	if (this->key == key)
		return true;
	else if (left && this->key > key)
		return left->find(key);
	else if (right && this->key < key)
		return right->find(key);
	return false;
}

void bst::insert(int key) {

}

void bst::del(int key) {

}

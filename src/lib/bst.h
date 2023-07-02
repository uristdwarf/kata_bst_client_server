struct bst {
	int key;
	bst *left;
	bst *right;
	bool find(int key);
	void insert(int key);
	void del(int key);
};

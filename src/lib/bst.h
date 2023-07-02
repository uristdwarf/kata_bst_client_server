#include <memory>
#include <stdexcept>

class bst_exception : public std::runtime_error {
	bst_exception(const std::string &msg) : std::runtime_error(msg) {};
};

struct bst {
	int key;
	std::shared_ptr<bst> left;
	std::shared_ptr<bst> right;
	bool find(int key);
	void insert(int key);
	void del(int key);
};

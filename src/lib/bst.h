#include <exception>
#include <memory>
#include <stdexcept>

class bst_exception : public std::runtime_error {
	public:
	bst_exception(const std::string &msg) : std::runtime_error(msg) {};
};

class last_node : public std::exception {

};

struct bst {
	int key;
	std::shared_ptr<bst> left;
	std::shared_ptr<bst> right;
	bst() {};
	bst(int number);
	bool find(int key);
	void insert(int key);
	void del(int key);
};

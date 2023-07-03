#include <arpa/inet.h>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <gtest/gtest.h>
#include <sys/socket.h>
#include "../src/lib/socket.h"
#include "../src/lib/bst.h"

TEST(SocketTests, CreateRemoteSocket) {
	string ip = "127.0.0.1";
	string port = "5510";
	connection conn(ip, port);

	auto info = conn.get_info();
	
	struct sockaddr_in *info_addr = (struct sockaddr_in*)info.ai_addr;

	auto ip_result = inet_ntoa(info_addr->sin_addr);
	EXPECT_EQ(ip_result, ip);
}

class BinarySearchTreeTests : public ::testing::Test {
	protected:
	bst *tree;

	void SetUp() override {
		tree = new bst;
		tree->key = 6;

		tree->left = new bst;
		tree->left->parent = tree;
		tree->left->left = new bst;
		tree->left->left->parent = tree->left;
		tree->left->right = new bst;
		tree->left->right->parent = tree->left;

		tree->right = new bst;
		tree->right->parent = tree;
		tree->right->right = new bst;
		tree->right->right->parent = tree->right;
		tree->right->left = new bst;
		tree->right->left->parent = tree->right;

		tree->left->key = 3;
		tree->left->left->key = 2;
		tree->left->right->key = 5;

		tree->right->key = 9;
		tree->right->right->key = 15;
		tree->right->left->key = 7;
	}
};

TEST_F(BinarySearchTreeTests, SearchKey) {
	ASSERT_TRUE(tree->find(5));
	ASSERT_FALSE(tree->find(20));
}

TEST_F(BinarySearchTreeTests, InsertKey) {
	int key = 20;
	tree->insert(key);
	ASSERT_TRUE(tree->right->right->right);
	ASSERT_EQ(tree->right->right->right->key, key);
	key = 1;
	tree->insert(key);
	ASSERT_TRUE(tree->left->left->left);
	ASSERT_EQ(tree->left->left->left->key, key);
	EXPECT_THROW({
			tree->insert(20);
	}, bst_exception);
}

TEST_F(BinarySearchTreeTests, DeleteKey) {
	int key = 2;
	tree->del(key);
	ASSERT_FALSE(tree->left->left);

	key = 3;
	tree->del(key);
	ASSERT_EQ(tree->left->key, 5);

	key = 20;
	tree->right->right->right = new bst;
	tree->right->right->right->key = 20;
	tree->right->right->right->parent = tree->right->right;
	tree->del(9);
	ASSERT_TRUE(tree->right);
	ASSERT_EQ(tree->right->key, 15);
	
	key = 6;
	tree->del(key);
	ASSERT_EQ(tree->key, 7);
	ASSERT_FALSE(tree->right->left);
	EXPECT_THROW({
			tree->del(key);
	}, bst_exception);
}

TEST(BinarySearchTreeIntegrationTest, InsertFindDelete) {
	auto tree = make_shared<bst>(10);
	tree->insert(5);
	ASSERT_TRUE(tree->find(5));
	ASSERT_TRUE(tree->find(10));
	ASSERT_FALSE(tree->find(15));
	tree->insert(15);
	ASSERT_TRUE(tree->find(15));
	tree->del(5);
	ASSERT_FALSE(tree->find(5));
	tree->del(10);
	ASSERT_FALSE(tree->find(10));
	EXPECT_THROW({tree->del(10);}, bst_exception);
	ASSERT_EQ(tree->key, 15);
	EXPECT_THROW({tree->del(15);}, last_node);
}

TEST(BinarySearchTreeIntegrationTest, BigInsertFindDelete) {
	// Testing random values from 1 to 100
	srand(time(NULL));
	int rand_val = rand() % 100 + 1;
	set<int> values({rand_val});
	// Insertion
	auto tree = make_shared<bst>(rand_val);
	for (int i = 0; i < 99; i++) {
		while (values.contains(rand_val)) {
			rand_val = rand() % 100 + 1;
		}
		tree->insert(rand_val);
		values.insert(rand_val);
	}
	// Find
	for (const int &v : values) {
		EXPECT_TRUE(tree->find(v)) << "value of " << v << " not found in tree";
	}
	// Delete
	for (const int &v : values) {
		tree->del(v);
		EXPECT_FALSE(tree->find(v)) << "value of " << v << " still found in tree";
	}

}

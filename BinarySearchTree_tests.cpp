// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
#include <iostream>
#include <string>
#include <string.h>


TEST(testing_tree_empty_and_full) {
    BinarySearchTree<int> tree;

    ASSERT_TRUE(tree.empty());
    ASSERT_EQUAL(tree.height(), 0);
    ASSERT_EQUAL(tree.size(), 0);


    tree.insert(4);
    tree.insert(3);
    tree.insert(5);


    ASSERT_FALSE(tree.empty());

    ASSERT_TRUE(tree.check_sorting_invariant());

    ASSERT_EQUAL(tree.height(), 2);

    ASSERT_EQUAL(tree.size(), 3);
}

TEST(testing_equal_too_etc) {
    BinarySearchTree<int> tree;
    BinarySearchTree<int> tree2;

    ASSERT_TRUE(tree2.empty());
    ASSERT_TRUE(tree.empty());
    ASSERT_EQUAL(tree.height(), 0);


    tree.insert(4);
    tree.insert(3);
    tree.insert(5);
    tree.insert(6);
    tree.insert(1);
    tree.insert(7);

    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree2.empty());
    ASSERT_EQUAL(tree.size(), 6);
    ASSERT_EQUAL(tree.height(), 4);

    tree2.insert(5);
    tree2.insert(2);
    tree2.insert(6);
    tree2.insert(3);
    ASSERT_FALSE(tree.empty());
    ASSERT_FALSE(tree2.empty());

    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(tree.check_sorting_invariant());

    ASSERT_EQUAL(tree2.height(), 3);

    ASSERT_EQUAL(*tree2.max_element(), 6);
    ASSERT_EQUAL(*tree.max_element(), 7);
    ASSERT_EQUAL(*tree2.min_element(), 2);
    ASSERT_EQUAL(*tree.min_element(), 1);

    tree = tree2;

    ASSERT_EQUAL(tree2.height(), 3);
    ASSERT_EQUAL(tree.height(), 3);
    ASSERT_EQUAL(*tree.max_element(), 6);
    ASSERT_EQUAL(*tree2.max_element(), 6);
    ASSERT_EQUAL(*tree2.min_element(), 2);
    ASSERT_EQUAL(*tree.min_element(), 2);

    ASSERT_FALSE(tree.empty());
    ASSERT_FALSE(tree2.empty());


    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(tree2.check_sorting_invariant());
    std::string printing;
}

TEST(min_greater_than_and_other_tests){
    BinarySearchTree<int> tree;

    tree.insert(4);
    tree.insert(6);
    tree.insert(2);
    tree.insert(3);
    tree.insert(7);

    ASSERT_EQUAL(*tree.min_greater_than(3), 4);
    ASSERT_EQUAL(*tree.min_greater_than(3), 4);
    ASSERT_EQUAL(*tree.begin(), 2);

}

TEST(test_sorting_invarient_for_solid_code){
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(2);
    tree.insert(10);
    BinarySearchTree<int>::Iterator it1;
    it1 = tree.begin();

    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_EQUAL(*it1, 2);
    it1++;
    ASSERT_EQUAL(*it1, 5);

    *tree.begin() = 8;
    ASSERT_FALSE(tree.check_sorting_invariant());

    tree.traverse_inorder(std::cout);
    tree.traverse_preorder(std::cout);
}

TEST(in_order_test) {
    std::ostringstream os;
    std::ostringstream os2;

    BinarySearchTree<int> tree;
    tree.insert(3);
    tree.insert(1);
    tree.insert(2);
    tree.insert(4);

    std::string expected = "1 2 3 4 ";
    tree.traverse_inorder(os);
    ASSERT_EQUAL(os.str(), expected);

    tree.traverse_preorder(std::cout);
    std::string expected_out = "3 1 2 4 ";
    tree.traverse_preorder(os2);
    ASSERT_EQUAL(os2.str(), expected_out);
}

TEST(testing_iterator_stuff){
    BinarySearchTree<int>::Iterator it1;
    BinarySearchTree<int>::Iterator it2;
    BinarySearchTree<int> tree;
    BinarySearchTree<int> tree2;

    it1 = tree.end();
    it2 = tree.max_element();
    ASSERT_EQUAL(it1, it2);


    tree.insert(3);
    tree.insert(2);
    tree.insert(4);

    it1 = tree.begin();
    it2 = tree.begin();

    tree2 = tree;
    bool are_trees_equal = false;
    if (it1 == it2){
        are_trees_equal = true;
    }
    ASSERT_TRUE(are_trees_equal);

    it2 = tree2.begin();

    are_trees_equal = false;
    if (it1 == it2){
        are_trees_equal = true;
    }
    ASSERT_FALSE(are_trees_equal);

    if (it1 != it2){
        are_trees_equal = true;
    }
    ASSERT_TRUE(are_trees_equal);

    it2 = tree.end();

    std::ostringstream os;

    //std::cout << tree << std::endl;
    os << tree;
    std::string expected_out = "[ 2 3 4 ]";
    ASSERT_EQUAL(os.str(), expected_out);
}

TEST_MAIN()

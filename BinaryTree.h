#ifndef _INCLUDE_BINARY_TREE_H_
#define _INCLUDE_BINARY_TREE_H_
#include <cstddef>
#include <iostream>
using namespace std;
class BinarySearchTree
{
    private:
        struct tree_node
        {
           tree_node* left;
           tree_node* right;
           int data;
        };
        tree_node* root;
    
    public:
        BinarySearchTree()
        {
           root = NULL;
        }
       
        bool isEmpty() const { return root==NULL; }
        void print_inorder();
        void inorder(tree_node*);
        void print_preorder();
        void preorder(tree_node*);
        void print_postorder();
        void postorder(tree_node*);
        void insert(int);
        void remove(int);
};


#endif
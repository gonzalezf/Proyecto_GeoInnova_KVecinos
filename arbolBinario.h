#ifndef _INCLUDE_ARBOL_BINARIO_H_
#define _INCLUDE_ARBOL_BINARIO_H_

struct node
{
  int key_value;
  node *left;
  node *right;
  int dimension;
};

class btree
{
    public:
        btree();
        ~btree();

        void insert(int key);
        node *search(int key);
        void destroy_tree();

    private:
        void destroy_tree(node *leaf);
        void insert(int key, node *leaf);
        node *search(int key, node *leaf);
        
        node *root;
};

#endif
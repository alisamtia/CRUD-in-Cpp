#pragma once
#include "components.h"
#include <vector>


// Binary search tree nodes
struct Bst_Node {
    Bst_Node* left;
    int id;
    Component* component_pointer;
    Bst_Node* right;
};

extern Bst_Node* root;


// Tree operations
void insert(Bst_Node*& tree, int id, Component* component_pointer);
void construct_tree(Bst_Node*& tree, std::vector<Component>& components, std::vector<Component>::iterator left, std::vector<Component>::iterator right);

void clear_tree(Bst_Node*& tree);
void simple_tree(Bst_Node*& tree, int depth=0, std::string side=" ");
struct Bst_Node* search(Bst_Node*& tree, int to_find);
struct Bst_Node* del(Bst_Node*& tree, int value);
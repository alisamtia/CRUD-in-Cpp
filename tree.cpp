#include "tree.h"
#include "components.h"
#include <iostream>


Bst_Node* root = nullptr;

/*
The trees in here points to the vector elements on purpose
1. It is unconventional but my target was only to learn how to use trees basically
2. It saves a lot of memory especially when the data is in sync
3. It is simple and easy approach
*/

// Insert Data into the binary search tree
void insert(Bst_Node*& tree, int id, Component* component_pointer){
    if(tree == nullptr){
        tree = new Bst_Node;
        tree->id = id;
        tree->component_pointer = component_pointer;
        tree->left = nullptr;
        tree->right = nullptr;

        return;
    }
    else if(id > tree->id){
        return insert(tree->right, id, component_pointer);
    }
    else{
        return insert(tree->left, id, component_pointer);
    }
}

// Search for the id of component from Binary Search Tree
struct Bst_Node* search(Bst_Node*& tree, const int to_find){
    if(tree == nullptr){
        return nullptr;
    }

    if(to_find == tree->id){
        return tree;
    }

    if(to_find > tree->id){
        return search(tree->right, to_find);
    }else{
        return search(tree->left, to_find);
    }
}

// Deletion from the Binary Search Tree
struct Bst_Node* del(Bst_Node*& tree, int value){
    if(tree == nullptr){
        return nullptr;
    }
    if(value != tree->id){
        if(value > tree->id){
            tree->right = del(tree->right, value);
            return tree;
        }else{
            tree->left = del(tree->left, value);
            return tree;
        }
    }

    // node is found

    // Case 1: Both sides are NULL, delete the tree and return Null
    // returning null ensures the parent_node->current_tree will become null
    if(tree->left == nullptr && tree->right == nullptr){
        delete tree;
        return nullptr;
    }


    // Case 2: One side is null we will return the ptr of non-null side
    // So the parent tree has delete tree's  sub tree(s)
    else if(tree->left == nullptr || tree->right == nullptr){
        Bst_Node* tmp;

        if(tree->left == nullptr){
            tmp = tree->right;
        }else{
            tmp = tree->left;
        }
        
        delete tree;
        return tmp;
    }

    // Case 3: Both sides are non-null replace current tree
    // with the successor
    else{
        // Find the successor
        Bst_Node* successor = tree->right;

        while(successor->left != nullptr){
            successor = successor->left;
        }

        // store successor value in a temporary variable
        int tmp_successor = successor->id;
        Component* tmp_componentPointer = successor->component_pointer;

        // delete selected successor leaf node
        del(tree, successor->id);

        // Replace the current value with the temporary successor value
        tree->id = tmp_successor;
        tree->component_pointer = tmp_componentPointer;

        // return the orignal tree with replaced value
        return tree;
    }
    
    // simply return the current tree itself no changes required
    return tree;
}

// Print the tree with all of it's nodes
void simple_tree(Bst_Node*& tree, int depth, std::string side){
    if(tree == nullptr){
        return;
    }

    if(depth == 0){
        std::cout << "Root: " << tree->id << std::endl << std::endl;
        std::cout << tree->id << std::endl;
    }
    for(int i=0; i < depth; i++){
        if((depth - i) == 1){
            if(side == "Left"){
                std::cout << "|--";
            }else{
                std::cout << "|__";
            }
        }else{
            std::cout << "| ";
        }
    }
    if(depth != 0){
        std::cout << side << " -> " << tree->id << std::endl;
    }

    
    simple_tree(tree->left, depth+1 , "Left");
    simple_tree(tree->right, depth+1 , "Right");
}

// convert a sorted vector into a binary search tree (BST)
void construct_tree(Bst_Node*& tree, std::vector<Component>& components, std::vector<Component>::iterator left, std::vector<Component>::iterator right){
    if(left ==  right){
        return;
    }
    auto mid = left + (right - left) / 2;

    insert(tree, mid->id, &(*mid));
    construct_tree(tree, components, mid + 1, right);
    construct_tree(tree, components, left, mid);
}

// AI-assisted change (Copilot SDK in VS Code): release all dynamically
// allocated tree nodes before rebuilding or exiting.
void clear_tree(Bst_Node*& tree){
    if(tree == nullptr){
        return;
    }

    clear_tree(tree->left);
    clear_tree(tree->right);
    delete tree;
    tree = nullptr;
}
#include <stack>
#include <iostream>
#include "UndoActions.h"
#include "components.h"
#include "tree.h"

std::stack<action> actions;

// Protoypes to be used only inside this file
void undo_insert(int old_component);
void undo_delete(Component old_component, int last_index_in_vector);
void undo_stock_update(int id, int old_stock);


void undo_last_action(){
    if(actions.empty()){
        std::cout << "No actions to undo" << std::endl;
        return;
    }

    action last_action = actions.top();
    std::cout << "Undoing last Action" << std::endl; 
    
    // Check for last action Type
    switch(last_action.type){
        case(INSERT):
            std::cout << "Component Deleted: " << last_action.component.name << std::endl; 
            undo_insert(last_action.component.id);
            break;
        case(DELETE):
            std::cout << "Component Added: " << last_action.component.name << std::endl; 
            undo_delete(last_action.component, last_action.last_index_in_vector);
            break;
        case(UPDATE_STOCK):
            std::cout << "Update " << last_action.component.name << " stock to " << last_action.component.stock << std::endl; 
            undo_stock_update(last_action.component.id, last_action.component.stock);
            break;
    }
    
    // delete the last action
    actions.pop();
};

// get id and undo insert
// AI-assisted change (Copilot SDK in VS Code): guard missing IDs and rebuild
// the tree after the component vector is erased.
void undo_insert(int id){
    // find component index
    int component_index = binary_search(id, components.begin(), components.end());

    if(component_index == -1){
        std::cout << "Unable to undo: component was not found" << std::endl;
        return;
    }

    // delete component from vector
    components.erase(components.begin() + component_index);

    clear_tree(root);
    construct_tree(root, components, components.begin(), components.end());
}

// create a duplicate of component insert it into vector as well as tree
// AI-assisted change (Copilot SDK in VS Code): validate the restore position
// and rebuild the tree to avoid pointers into the local component copy.
void undo_delete(Component old_component, int last_index_in_vector){
    if(last_index_in_vector < 0 ||
       last_index_in_vector > static_cast<int>(components.size())){
        std::cout << "Unable to undo: invalid component position" << std::endl;
        return;
    }

    // insert at the old index to keep the vector stable as well as arranged in ascending order
    components.insert(components.begin() + last_index_in_vector ,old_component);

    clear_tree(root);
    construct_tree(root, components, components.begin(), components.end());
}

void undo_stock_update(int id, int old_stock){
    Bst_Node* node = search(root, id);
    // AI-assisted change (Copilot SDK in VS Code): avoid dereferencing a
    // missing node if inventory and undo history are inconsistent.
    if(node == nullptr){
        std::cout << "Unable to undo: component was not found" << std::endl;
        return;
    }
    node->component_pointer->stock = old_stock;
}
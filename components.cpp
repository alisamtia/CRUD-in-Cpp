#include "components.h"
#include "helpers.h"
#include "tree.h"
#include "UndoActions.h"
#include <iostream>
#include <limits>

std::vector<Component> components;

// Private function prototypes
void create_undo_action(Component *component, ActionType type, int last_index_in_vector = -1);

// AI-assisted change (Copilot SDK in VS Code): keep the enum conversion's
// fallback as the switch default rather than unreachable code after the switch.
// Component Type(int) to string
std::string component_type(ComponentType type){
    switch(type){
        case (0): return "CPU";
        case (1): return "GPU";
        case (2): return "RAM";
        case (3): return "STORAGE";
        case (4): return "MOTHERBOARD";
        case (5): return "PSU";
        default:  return "OTHER";
    }
}

// component type String to Type
ComponentType component_type(std::string type){
    if(type == "CPU")         {return CPU;}
    if(type == "GPU")         {return GPU;}
    if(type == "RAM")         {return RAM;}
    if(type == "STORAGE")     {return STORAGE;}
    if(type == "MOTHERBOARD") { return MOTHERBOARD;}
    if(type == "PSU")         { return PSU; }

    std::cout << "Invalid CSV File; Falling back to 'OTHER' Type" << std::endl;
    return OTHER;
}

// show a component on the terminal
void show_component(const Component& component){
    std::cout << "Id:    " << component.id << std::endl;
    std::cout << "Name:  " << component.name << std::endl;
    std::cout << "Price: $" << component.price << std::endl;
    std::cout << "Type:  " << component_type(component.type) << std::endl;
    std::cout << "Stock: " << component.stock << std::endl;
}

// linear search due to unsorted data
void search_component(std::string name){
    bool not_found=true;
    name=tolower(name);
    for(const Component& component: components){
        // make it case insensitive
        std::string component_name=tolower(component.name);
        if(component_name.find(name) != std::string::npos){
            show_component(component);
            not_found=false;
        }
    }
    if(not_found){
        std::cout << "No Component Found!" << std::endl;
    }
}

// Display all components available on the terminal
void show_all_components(){
    if (components.empty()){
        std::cout << "No Components Available!";
        return;
    }
    // AI-assisted change (Copilot SDK in VS Code): avoid copying each record
    // just to display it.
    for(const Component& component: components){
        show_component(component);
        std::cout << std::endl;
    };
}

// Select a component type while adding a new component
ComponentType select_component_type(){
    std::cout << "1: CPU" << std::endl;
    std::cout << "2: GPU" << std::endl;
    std::cout << "3: RAM" << std::endl;
    std::cout << "4: STORAGE" << std::endl;
    std::cout << "5: MOTHERBOARD" << std::endl;
    std::cout << "6: PSU" << std::endl;

    while(true){
        int component_number=recurring_input(1, "Enter Component Type: ", 6);

        switch(component_number){
            case (1):   return CPU;
            case (2):   return GPU;
            case (3):   return RAM;
            case (4):   return STORAGE;
            case (5):   return MOTHERBOARD;
            case (6):   return PSU;
        }
    }
}

// AI-assisted change (Copilot SDK in VS Code): validate names and keep BST
// component pointers valid after vector storage changes.
// Add a new component
void add_component(Bst_Node*& tree){
    Component component;
    
    std::string name;
    std::cout << "Enter Component Name: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);
    while(name.empty()){
        std::cout << "Component name cannot be empty." << std::endl;
        std::cout << "Enter Component Name: ";
        std::getline(std::cin, name);
    }
    component.name=name;
    
    int price=recurring_input(1, "Enter Component Price: ");
    component.price=price;

    component.type=select_component_type();
    
    int stock=recurring_input(1, "Enter Component Stock: ");
    component.stock=stock;

    component.id = 0;
    if(!components.empty()){
        component.id = components.back().id + 1;
    }

    components.push_back(component);

    // Rebuild pointers after vector storage may have moved.
    clear_tree(tree);
    construct_tree(tree, components, components.begin(), components.end());

    // insert it into the actions stack
    create_undo_action(&component, INSERT);
    
    return;
}


// Update the number of stocks of component
void update_stock(Bst_Node*& tree){
    int component_id = recurring_input(0, "Enter Component Id: ");
    Bst_Node* component_node = search(tree, component_id);

    if(component_node == nullptr){
        std::cout << "Component Not found!";
        return;
    }
    
    // Component Found
    Component* component = component_node->component_pointer;
    std::cout << "Total Stock: " << component->stock << std::endl;
    int new_stock = recurring_input(0, "Enter New Stock: ");

    create_undo_action(component, UPDATE_STOCK);

    component->stock = new_stock;
    return;
}

void create_undo_action(Component *component, ActionType type, int last_index_in_vector)
{
    action insert_action;
    insert_action.type = type;
    insert_action.component = *component;
    insert_action.last_index_in_vector = last_index_in_vector;
    actions.push(insert_action);
}

// Binary search the vector of components
int binary_search(int id, std::vector<Component>::iterator left, std::vector<Component>::iterator right){
    if(left == right){
        return -1;
    }
    // we add left to make total like
    // half is 3
    // by mid distance from the vector will become after sum by left like
    // 73 which is real index in vector to index and compare
    auto mid = left + (right - left) / 2;
    
    int current_id = mid->id;

    if(current_id == id){
        // return current element index
        return mid - components.begin();
    }
    else if(id < current_id){
        return binary_search(id, left, mid);
    }
    else if(id > current_id){
        return binary_search(id, mid + 1, right);
    }
    return -1;
}




// Find component using binary search and delete that component
// AI-assisted change (Copilot SDK in VS Code): rebuild the BST after vector
// erasure so tree pointers refer to the current component locations.
void remove_component(Bst_Node*& tree){
    // int component_id = recurring_input(0, "Enter Component Id: ", (components.size()-1));
    int component_id = recurring_input(-9999, "Enter Component Id: ");
    // delete the component if it exists
    int component_index=binary_search(component_id, components.begin(), components.end());

    if(component_index == -1){
        std::cout << "Component Not Found !!";
        return;
    }

    std::cout << "Deleted Item: " << components[component_index].name << std::endl;
    // AI-assisted change (Copilot SDK in VS Code): rebuild the BST after erase
    // because erasing shifts component addresses in the vector.
    // delete it from the BST
    del(tree, components[component_index].id);

    // Add into actions to undo action
    create_undo_action(&components[component_index], DELETE, component_index);

    // Erase(Delete) it from the main tree
    components.erase(components.begin() + component_index);

    // Rebuild pointers after vector elements shifted.
    clear_tree(tree);
    construct_tree(tree, components, components.begin(), components.end());
}

// Show total inventory value and total number of Components
// AI-assisted change (Copilot SDK in VS Code): widen multiplication and total
// to reduce overflow risk for larger inventories.
void inventory_value(){
    long long total_value=0;
    for(const Component& c : components){
        total_value += static_cast<long long>(c.stock) * c.price;
    }
    std::cout << "Inventory Value:   $" << total_value << std::endl;
    std::cout << "Total Different Components: " << components.size() << std::endl;
}
#pragma once
#include <string>
#include <vector>

struct Bst_Node;

enum ComponentType {
    CPU,
    GPU,
    RAM,
    STORAGE,
    MOTHERBOARD,
    PSU,
    OTHER
};

struct Component{
    int id;
    std::string name;
    int price;
    ComponentType type;
    int stock;
};

extern std::vector<Component> components;

int binary_search(int id, std::vector<Component>::iterator left, std::vector<Component>::iterator right);
void show_component(const Component& component);
std::string component_type(ComponentType type);
ComponentType component_type(std::string type);
void search_component(std::string name);
ComponentType select_component_type();
void show_all_components();
void remove_component(Bst_Node*& tree);
void inventory_value();
void add_component(Bst_Node*& tree);
void update_stock(Bst_Node*& tree);

#include <iostream>
#include <string>
#include <limits>

#include "helpers.h"
#include "components.h"
#include "FileManager.h"
#include "tree.h"
#include "UndoActions.h"

// Prototypes
int display_menu();
void select_from_menu();

// AI-assisted change (Copilot SDK in VS Code): support a default/custom CSV
// path, normalize its extension, and release tree nodes at shutdown.
int main(int argc, char* argv[]) {
    std::string file_name = argc == 2 ? argv[1] : "components.csv";
    if(argc > 2){
        std::cout << "Usage: ./main [filename.csv]" << std::endl;
        return 1;
    }

    if(file_name.size() < 4 ||
       tolower(file_name.substr(file_name.size() - 4)) != ".csv"){
        file_name += ".csv";
    }

    bool imported = import_components(file_name);
    if(!imported){
        return 1;
    }
    construct_tree(root, components, components.begin(), components.end());
    
    std::cout << "==== PC SHOP ====\n" << std::endl;
    select_from_menu();
    clear_tree(root);

    return 0;
}

// Select the input user chose and call corresponding function
void select_from_menu(){
    std::string search_name;
    std::string file_name;
    Bst_Node* searched_node;
    int id;
    while (true){
        int selected=display_menu();
        switch(selected){
            case (1):
                show_all_components();
                break;
            case (2):
                std::cout << "Search> ";
                
                // AI-assisted change (Copilot SDK in VS Code): discard the
                // pending newline and exit cleanly if input reaches EOF.
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if(!std::getline(std::cin, search_name)){
                    std::cout << "Input ended. Exiting." << std::endl;
                    return;
                }
                search_component(search_name);
                break;
            case (3):            
                add_component(root);
                break;
            case (4):            
                remove_component(root);
                break;
            case (5):            
                update_stock(root);
                break;
            case (6):
                inventory_value();
                break;
            case (7):
                id = recurring_input(0, "Enter Id: ");
                searched_node = search(root, id);
                if(searched_node != nullptr){
                    show_component(*searched_node->component_pointer);
                }else{
                    std::cout << "Component Not Found!" << std::endl;
                }
                break;
            case (8):
                simple_tree(root);
                break;
            case (9):
                // Undo Last action
                undo_last_action();
                break;
            case (10):
                export_components();
                break;
            case (11):
                return;
            default:
                std::cout << "Invalid Value";
                break;
        }
        std::cout << std::endl;
    }
}


// Display Menu and get user Input from select menu
int display_menu(){
    do{
        std::cout << "1. View All Components" << std::endl;
        std::cout << "2. Search Components" << std::endl;
        std::cout << "3. Add Component" << std::endl;
        std::cout << "4. Remove Component" << std::endl;
        std::cout << "5. Update Stock" << std::endl;
        std::cout << "6. Show Inventory Value" << std::endl;
        std::cout << "7. Fast Id Lookup" << std::endl;
        std::cout << "8. Display Tree" << std::endl;
        std::cout << "9. Undo Last Action" << std::endl;
        std::cout << "10. Save File" << std::endl;
        std::cout << "11. Exit \n" << std::endl;

        int selected;
        std::cout << "Select: ";
        // AI-assisted change (Copilot SDK in VS Code): handle invalid input
        // and EOF without leaving the menu in a failed-stream loop.
        if(!(std::cin >> selected)){
            if(std::cin.eof()){
                std::cout << "Input ended. Exiting." << std::endl;
                return 11;
            }
            std::cout << "That's not a number!" << std::endl;
            clearInput();
            continue;
        }
        if(selected < 1 or selected > 11){
            continue;
        }
        return selected;

        } while(true);
}

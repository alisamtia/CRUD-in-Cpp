#include "FileManager.h"
#include "components.h"

#include <iostream>
#include <sstream>
#include <fstream> // file stream
#include <cctype>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>


// AI-assisted change (Copilot SDK in VS Code): check output open, write, and
// close states so export failures are not reported as successes.
// Export the components into a single file
void export_components(){
    std::string file_name;
    std::cout << "Enter File Name (Without extention): ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, file_name);
    if(file_name.empty()){
        std::cout << "File name cannot be empty." << std::endl;
        return;
    }

    std::ofstream exporting_file(file_name + ".csv", std::ios::trunc);

    if(!exporting_file.is_open()){
        std::cout << "Unable to create file: " << file_name + ".csv" << std::endl;
        return;
    }

    exporting_file << "id,name,price,type,stock\n";

    for(const Component& c : components){
        exporting_file << c.id << ","
                       << c.name << ","
                       << c.price << ","
                       << component_type(c.type) << ","
                       << c.stock
                       << '\n';
    }

    if(!exporting_file.good()){
        std::cout << "Export failed while writing: " << file_name + ".csv" << std::endl;
        return;
    }

    exporting_file.close();
    if(exporting_file.fail()){
        std::cout << "Export failed while closing: " << file_name + ".csv" << std::endl;
        return;
    }

    std::cout << "Components Exported to: " << file_name + ".csv" << std::endl;
}

// AI-assisted change (Copilot SDK in VS Code): validate CSV header and rows,
// handle BOM/CRLF input, and publish imported data only after successful parsing.
// import components from a file
bool import_components(std::string file_path){
    std::ifstream components_file(file_path);

    if(!components_file.is_open()){
        std::cout << "Can't open the file " << file_path << std::endl;
        return false;
    }

    std::string line;
    std::vector<Component> imported_components;

    if(!std::getline(components_file, line)){
        std::cout << "CSV file has an invalid header." << std::endl;
        return false;
    }
    if(!line.empty() && line.back() == '\r'){
        line.pop_back();
    }
    const std::string utf8_bom = "\xEF\xBB\xBF";
    if(line.compare(0, utf8_bom.size(), utf8_bom) == 0){
        line.erase(0, utf8_bom.size());
    }
    if(line != "id,name,price,type,stock"){
        std::cout << "CSV file has an invalid header." << std::endl;
        return false;
    }

    int previous_id = -1;
    while(std::getline(components_file, line)){
        if(!line.empty() && line.back() == '\r'){
            line.pop_back();
        }
        if(line.empty()){
            continue;
        }
        std::stringstream ss(line);
        
        std::string id;
        std::string name;
        std::string price;
        std::string type;
        std::string stock;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, price, ',');
        getline(ss, type, ',');
        getline(ss, stock);

        Component component;

        try{
            std::size_t id_position = 0;
            std::size_t price_position = 0;
            std::size_t stock_position = 0;
            component.id = std::stoi(id, &id_position);
            component.name = name;
            component.price = std::stoi(price, &price_position);
            component.type = component_type(type);
            component.stock = std::stoi(stock, &stock_position);
            if(id_position != id.size() ||
               price_position != price.size() ||
               stock_position != stock.size() ||
               component.id < 0 ||
               component.id <= previous_id ||
               component.price < 0 ||
               component.stock < 0 ||
               component.name.empty()){
                throw std::invalid_argument("invalid component data");
            }
            previous_id = component.id;
            imported_components.push_back(component);
        } catch (const std::invalid_argument&) {
            std::cout << "CSV file has an invalid row: " << line << std::endl;
            return false;
        } catch (const std::out_of_range&) {
            std::cout << "CSV file has a number outside the supported range: " << line << std::endl;
            return false;
        }
    }
    components_file.close();
    components = std::move(imported_components);
    std::cout << "Components Imported from: " << file_path << std::endl;
    return true;
}
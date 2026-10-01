#include "helpers.h"
#include <cctype>
#include <string>
#include <iostream>
#include <limits>
#include <cstdlib>

// AI-assisted changes (Copilot SDK in VS Code): use streamsize limits,
// iterative validation, EOF handling, and safe character conversion below.
// Clear the input error and recent interferance from cout etc
void clearInput(){
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Get recurring and valid int as input only
int recurring_input(int min, std::string label, int max){
    while(true){
        int input;
        std::cout << label;
        if(!(std::cin >> input)){
            if(std::cin.eof()){
                std::cout << "Input ended. Exiting." << std::endl;
                std::exit(EXIT_SUCCESS);
            }
            std::cout << "Enter a valid Integer" << std::endl;
            clearInput();
            continue;
        }

        if(input < min){
            std::cout << "Enter a greater or equal to " << min << std::endl;
            continue;
        }
        if(input > max){
            std::cout << "Enter a less or equal to " << max << std::endl;
            continue;
        }
        return input;
    }
}

// Convert a string to lower characters
std::string tolower(std::string data){
    for(char& c : data){
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return data;
}
#!/bin/bash
# AI-assisted change (Copilot SDK in VS Code): explicitly select C++20 and
# enable compiler warnings for reproducible builds.
g++ -std=c++20 -Wall -Wextra -pedantic components.cpp tree.cpp helpers.cpp FileManager.cpp UndoActions.cpp main.cpp -o main
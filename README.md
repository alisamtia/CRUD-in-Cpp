# PC Shop Inventory

**Project title:** PC Shop Inventory System
**Demonstration video:** https://youtu.be/KdQ73stA8OA

PC Shop Inventory is a command-line inventory management application written in C++. It is designed for a small PC hardware shop and keeps track of components such as CPUs, GPUs, RAM, storage devices, motherboards, and power supplies. The program imports inventory data from a CSV file, lets a user manage that inventory through an interactive menu, and can export the updated data to another CSV file.

## Features

- Import components from a CSV file when the program starts
- Display all components
- Search components by name
- Add a new component
- Remove a component by ID
- Update the stock quantity of a component
- Calculate the total inventory value
- Perform fast component lookup by ID using a binary search tree
- Display the binary search tree
- Undo the most recent add, delete, or stock-update action
- Export the current inventory to a CSV file
- Validate integer input and reject invalid values

## Example component data

Each component contains:

- An ID
- A name
- A price
- A component type
- A stock quantity

Example:

```text
id,name,price,type,stock
0,Intel Core i3-12100F,799,CPU,15
21,NVIDIA GeForce RTX 3060,2299,GPU,8
38,Corsair Vengeance 16GB DDR4,499,RAM,18
```

## Requirements

You need:

- A C++ compiler with C++20 support
- A terminal or command prompt
- The project files

The program uses an abbreviated function template in `binary_search`, so C++20 support is required.

The project has been tested with `g++`.

## Building

### Using g++

From the project directory, run:

```bash
g++ -std=c++20 -Wall -Wextra -pedantic components.cpp tree.cpp helpers.cpp FileManager.cpp UndoActions.cpp main.cpp -o main
```

On Windows, the output file may be named `main.exe` automatically depending on the compiler configuration.

### Using the build script

The repository includes `build.sh`:

```bash
./build.sh
```

The script compiles all source files into an executable named `main`.

If the script does not run on your system, use the explicit `g++` command above.

## Running

Run the executable from the project directory:

```bash
./main
```

On Windows:

```powershell
.\main.exe
```

If no file is provided, the program loads `components.csv` from the current working directory. A custom CSV file can also be supplied; the `.csv` extension is optional:

```powershell
.\main.exe
.\main.exe components.csv
.\main.exe supplier-inventory
```

If the file cannot be opened or contains invalid data, the program displays an error and stops.

## Menu options

When the program starts, it displays the following menu:

```text
1. View All Components
2. Search Components
3. Add Component
4. Remove Component
5. Update Stock
6. Show Inventory Value
7. Fast Id Lookup
8. Display Tree
9. Undo Last Action
10. Save File
11. Exit
```

### View all components

Displays every component currently stored in memory.

### Search components

Searches component names without considering letter case. Partial names are supported. For example, searching for `ryzen` can display several AMD Ryzen components.

### Add component

Prompts for:

- Component name
- Price
- Component type
- Stock quantity

The program assigns a new ID to the component.

### Remove component

Removes a component using its ID and records the action so it can be undone.

### Update stock

Changes the stock quantity for a component selected by ID. The previous stock value is saved for undo.

### Show inventory value

Calculates the inventory value using:

```text
price * stock
```

It also displays the number of different component records.

### Fast ID lookup

Looks up a component by ID using the binary search tree instead of scanning every component.

### Display tree

Prints a simple text representation of the binary search tree used for ID lookup.

### Undo last action

Undo supports the most recent:

- Add operation
- Delete operation
- Stock update

Undo operations are stored in a stack, so the most recent action is undone first.

### Save file

Prompts for a file name and exports the current inventory as a CSV file. The `.csv` extension is added automatically.

For example, entering:

```text
updated-inventory
```

creates:

```text
updated-inventory.csv
```

## Project structure

| File              | Purpose                                                                                                                                                |
| ----------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `main.cpp`        | Starts the program and controls the menu                                                                                                               |
| `components.cpp`  | Defines the shared inventory and implements component display, name search, type conversion, adding, removing, stock updates, and inventory valuation. |
| `components.h`    | Declares the `Component` structure, component type enumeration, shared vector, and component-operation functions.                                      |
| `tree.cpp`        | Implements binary search tree insertion, lookup, deletion, display, construction from inventory, and memory cleanup.                                   |
| `tree.h`          | Defines a tree node and declares the global root and tree operations.                                                                                  |
| `UndoActions.cpp` | Implements the undo stack and reverses add, delete, and stock-update actions.                                                                          |
| `UndoActions.h`   | Defines the undo action types and snapshot structure, and declares the undo stack interface.                                                           |
| `FileManager.cpp` | Imports and validates inventory CSV data and exports the current inventory.                                                                            |
| `FileManager.h`   | Declares the CSV import and export functions.                                                                                                          |
| `helpers.cpp`     | Implements integer input validation, input-buffer cleanup, EOF handling, and case conversion.                                                          |
| `helpers.h`       | Declares the shared input and string helper functions.                                                                                                 |
| `components.csv`  | Supplies the default starting inventory used when no file argument is given.                                                                           |
| `build.sh`        | Compiles the source files with g++ in C++20 mode and enables compiler warnings.                                                                        |

## Data structures and algorithms

### Vector

The main inventory is stored in a `std::vector<Component>`. This makes it convenient to display all components, search component names, and calculate the inventory value.

### Binary search tree

The program constructs a binary search tree using component IDs. This provides a separate fast lookup path for searching by ID.

The tree nodes refer to the corresponding components in the vector. Because vector operations can move elements in memory, the tree is rebuilt after operations that may change vector storage or element positions.

### Stack

Undo actions are stored in a `std::stack<action>`. This follows the last-in, first-out behavior expected from an undo feature.

### Binary search

The component vector is expected to be ordered by ID. The program uses binary search to find a component's position when removing or undoing an insertion.

## Design choices

I used a vector as the main inventory because the program needs to display and process the complete list often. It is straightforward to loop over a vector to show components, search names, and calculate the total inventory value. A vector also made it possible to practice iterators and binary search. The tradeoff is that insertions and removals can move elements, so the tree's pointers into the vector can become invalid. To keep those pointers correct, the program rebuilds the tree after vector operations that may move or shift components. This is easy to explain and fits this learning project, although a larger application could use stable ownership or another indexing design to avoid rebuilding.

The binary search tree is included to practice implementing a data structure and provide a distinct ID-lookup feature. The inventory file is ordered by ID, allowing the tree to be initially constructed in a balanced shape by repeatedly selecting middle elements. The tree is kept separate from the vector because the vector remains the authoritative inventory collection; the tree is an index used to find components by ID.

I chose a stack for undo because undo naturally follows last-in, first-out order: the most recent change should be reversed first. Each action stores a snapshot of the affected component, and deletion actions also store the former vector position so the record can be restored in order.

The interface is text-based because the project's focus is C++ fundamentals, data structures, and file handling rather than graphics. A menu keeps the features accessible while allowing the program to remain small enough to understand as a first C++ project.

## CSV format

The input file should contain a header followed by rows with five comma-separated fields:

```text
id,name,price,type,stock
```

Supported component types are:

- `CPU`
- `GPU`
- `RAM`
- `STORAGE`
- `MOTHERBOARD`
- `PSU`

Unknown types are treated as `OTHER`.

The current CSV format is intended for simple component names that do not contain commas.
The importer accepts both Unix (LF) and Windows (CRLF) line endings, and accepts a UTF-8 BOM at the beginning of the header.

## Error handling

The program currently handles several common errors:

- Missing input CSV file
- Invalid integer input
- Numbers outside the allowed range
- Component IDs that cannot be found
- Attempting to undo when there are no actions
- Invalid output-file creation
- Output-file write failures

## Known limitations

- The CSV parser is simple and does not fully support quoted CSV fields or commas inside component names.
- The input file is expected to be ordered by ID for binary search to work correctly.
- The importer validates the header, required fields, numeric values, non-negative values, and ascending unique IDs.
- The tree depends on the vector which is a design choice, It can save memory is unconventional,
  I regerate tree when the vector changes.
- The program is intended for one user in a terminal and does not include accounts, authentication, networking, or a graphical interface.
- Inventory changes are saved only when the user explicitly exports them.

## AI assistance disclosure

An AI assistant using Copilot SDK in VS Code assisted with reviewing bugs,
implementing the code changes marked inline in the source files, and drafting
this README.

## Learning goals

This project was created to practice:

- C++ program organization across multiple source and header files
- Structs and enumerations
- Vectors and iterators
- Binary search
- Binary search trees
- Stacks
- Pointers and dynamic memory
- File input and output
- CSV-style data processing
- Input validation
- Undo functionality

## Author

Ali Samtia
Created as a first C++ project and as a practical exercise in data structures and file management.

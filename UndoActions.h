#include <stack>
#include "components.h"

enum ActionType{
    INSERT,
    DELETE,
    UPDATE_STOCK
};

struct action{
    ActionType type;
    Component component;
    int last_index_in_vector = -1;
};


extern std::stack<action> actions;

void undo_last_action();
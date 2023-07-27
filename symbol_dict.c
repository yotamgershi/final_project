#include "symbol_dict.h"

void insert(symbol_dict *symbols, char *name, int address, boolean is_data, boolean is_entry)
{
    int index = tolower(name[0]) - 'a';
    add_symbol(&(symbols->dict[index]), name, address, is_data, is_entry);
}

symbol_node *find(symbol_dict *symbols, char *name)
{
    int index = tolower(name[0]) - 'a';   
    return find_symbol(symbols->dict[index], name);
}


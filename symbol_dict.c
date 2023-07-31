#include "symbol_dict.h"

void insert(symbol_dict *symbols, char *name, int address, int line, boolean is_extern, boolean is_entry)
{
    int index;
    index = tolower(name[0]) - 'a';
    add_symbol(&(symbols->dict[index]), name, address,line, is_extern, is_entry);
}

symbol_node *find(symbol_dict *symbols, char *name)
{
    int index;
    if(!isalpha(*name))
        return NULL;
    index = tolower(name[0]) - 'a';   
    return find_symbol(symbols->dict[index], name);
}


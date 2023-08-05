#include "symbol_dict.h"

void insert(symbol_dict *symbols, char *name, int address, int line, int is_extern, int is_entry, int is_data)
{
    int index;
    index = tolower(name[0]) - 'a';
    add_symbol(&(symbols->dict[index]), name, address,line, is_extern, is_entry,is_data);
}

symbol_node *find(symbol_dict *symbols, char *name)
{
    int index;
    if(!isalpha(*name))
        return NULL;
    index = tolower(name[0]) - 'a';   
    return find_symbol(symbols->dict[index], name);
}

void free_dictionary_symbol(symbol_dict *symbols)
{
    int i;
    for(i = 0; i < NUM_OF_LETTERS; i++) 
    {
        free_symbol_list(symbols->dict[i]);
    }
    free(symbols);
}

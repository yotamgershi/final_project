#include "macro_dict.h"

void insert_macro_dict(macro_dict *macros, char *name, char *body)
{
    int index;
    index = tolower(name[0]) - 'a';
    add_macro(macros->dict + index, name, body);
}

macro_node *find_macro_dict(macro_dict *macros, char *name)
{
    int index;
    if(!isalpha(*name))
        return NULL;
    index = tolower(name[0]) - 'a';   
    return find_macro(macros->dict[index], name);
}
void free_dictionary(macro_dict *macros)
{
    int i;
    for(i = 0; i < NUM_OF_LETTERS; i++) 
    {
        free_macro_list(macros->dict[i]);
    }
    free(macros);
}


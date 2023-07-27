#include "macro_dict.h"

void insert_macro_dict(macro_dict *macros, char *name, char *def)
{
    int index = tolower(name[0]) - 'a';
    add_macro(macros->dict + index, name, def);
}

macro_node *find_macro_dict(macro_dict *macros, char *name)
{
    int index = tolower(name[0]) - 'a';   
    return find_macro(macros->dict[index], name);
}


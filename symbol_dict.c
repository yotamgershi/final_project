#include "symbol_dict.h"



struct symbol_node {
    struct symbol_node *next; /**< Pointer to the next symbol in the list. */
    char name[MAX_SYMBOL]; /**< The name of the symbol. */
    int address; /**< The address associated with the symbol. */
    int line_number;    /**< The line in "am" file of the symbol. */
    int is_extern; /**< Flag indicating whether the symbol is a extern symbol (TRUE) or not (FALSE). */
    int is_entry; /**< Flag indicating whether the symbol is an entry symbol (TRUE) or not (FALSE). */
    int is_data; /**< Flag indicating whether the symbol is a data symbol (TRUE) or not (FALSE). */
};

struct symbol_dict {
    symbol_node *dict[NUM_OF_LETTERS]; 
};


int get_address(symbol_node *symbol)
{
    return (symbol->address);
}


int get_is_entry(symbol_node *symbol)
{
    return (symbol->is_entry);
}


char *get_name(symbol_node *symbol)
{
    return (symbol->name);
}


int get_line_number(symbol_node *symbol)
{
    return (symbol->line_number);
}


int get_is_extern(symbol_node *symbol)
{
    return (symbol->is_extern);
}


int get_is_data(symbol_node *symbol)
{
    return (symbol->is_data);
}


symbol_node *get_next_symbol(symbol_node *symbol)
{
    return (symbol->next);
}


symbol_node *get_page(symbol_dict *dict,int page_number)
{
    return (dict->dict[page_number]);
}

symbol_dict *new_symbol_dict()
{
    return (symbol_dict*)validate_calloc_symbol(1,sizeof(symbol_dict));
}

void set_is_entry(symbol_node *symbol,int is_entry)
{
    symbol->is_entry=is_entry;
}

void set_is_data(symbol_node *symbol,int is_data)
{
    symbol->is_data=is_data;
}

void set_address(symbol_node *symbol,int address)
{
    symbol->address=address;
}


void insert(symbol_dict *symbols, char *name, int address, int line_number, int is_extern, int is_entry, int is_data)
{
    int index;
    index = tolower(name[0]) - 'a';
    add_symbol(&(symbols->dict[index]), name, address,line_number, is_extern, is_entry,is_data);
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





symbol_node *new_symbol()
{
    return (symbol_node*)validate_calloc_symbol(1, sizeof(symbol_node));
}



symbol_node *find_symbol(symbol_node *head, char *name)
{
    while (head)
    {
        if (!strcmp(head->name, name))
            return head;
        head = head->next;
    }
    return NULL;
}

void add_symbol(symbol_node **head, char *name, int address, int line_number ,int is_extern,int is_entry,int is_data)
{
    symbol_node *new = new_symbol();
    strcpy(new->name, name);
    new->next = *head;
    new->address = address;
    new->line_number = line_number;
    new->is_extern = is_extern;
    new->is_entry = is_entry;
     new->is_data = is_data;
    *head = new;
}

void free_symbol_list(symbol_node *head)
{

    while(head)
    {
        symbol_node *tmp = head;
        head = head->next;
        free(tmp);

    }
}

void *validate_calloc_symbol(size_t nitems, size_t size)
{
    void *mem = calloc(nitems, size);
    if (!mem)
    {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }
    return mem;
}



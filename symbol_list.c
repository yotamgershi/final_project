#include "symbol_list.h"

/* Structurs */
symbol_node *new_symbol()
{
    return (symbol_node*)validate_calloc(1, sizeof(symbol_node));
}

/* Functions */

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

void add_symbol(symbol_node **head, char *name, int address, int line, is_extern,int is_entry,int is_data)
{
    symbol_node *new = new_symbol();
    strcpy(new->name, name);
    new->next = *head;
    new->address = address;
    new->line = line;
    new->is_extern = is_extern;
    new->is_entry = is_entry;
     new->is_data = is_data;
    *head = new;
}
void *validate_calloc(size_t nitems, size_t size)
{
    void *mem = calloc(nitems, size);
    if (!mem)
    {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }
    return mem;
}

void *validate_malloc(size_t size)
{
    void *mem = malloc(size);
    if (!mem)
    {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }
    return mem;
}

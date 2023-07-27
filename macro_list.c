#include "macro_list.h"

/* Structurs */
macro_node *new_macro()
{
    return (macro_node*)validate_calloc(1, sizeof(macro_node));
}

macro_node *find_macro(macro_node *head, char *name)
{
    while (head)
    {
        if (!strcmp(head->name, name))
            return head;
        head = head->next;
    }
    return NULL;
}

/* Functions */
void add_macro(macro_node **head, char *name, char *def)
{
    macro_node *new = new_macro();
    strcpy(new->name, name);
    new->def = def;
    new->next = *head;
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

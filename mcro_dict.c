#include "mcro_dict.h"



/*dictionary of mcros, every page contain list of the mcros that starts with the letter of the page,
  for exemple page 1 contain every mcro starts with a(or A)*/
struct mcro_dict {
    mcro_node *dict[NUM_OF_LETTERS];
};

struct mcro_node {
    struct mcro_node *next; /*  Pointer to the next mcro node in the list. */
    char name[MAX_SYMBOL]; /* The name of the mcro. */
    char *body; /* The body of the mcro. */
};


mcro_dict *new_dict()
{
     return (mcro_dict*)validate_calloc(1, sizeof(mcro_dict));
}

/* receive dictionary of mcros, mcro name and body and insert to the dictionary in the right page*/
void insert_mcro_dict(mcro_dict *mcros, char *name, char *body)
{
    int index;
    index = tolower(name[0]) - 'a';
    add_mcro(mcros->dict + index, name, body);
}

/* receive dictionary of mcros, and mcro name, return pointer to mcro if its in the dict or NULL elsewise*/
mcro_node *find_mcro_dict(mcro_dict *mcros, char *name)
{
    int index;
    if(!isalpha(*name))
        return NULL;
    index = tolower(name[0]) - 'a';   
    return find_mcro(mcros->dict[index], name);
}

void free_dictionary(mcro_dict *mcros)
{
    int i;
    for(i = 0; i < NUM_OF_LETTERS; i++) 
    {
        free_mcro_list(mcros->dict[i]);
    }
    free(mcros);
}



mcro_node *new_mcro()
{
    return (mcro_node*)validate_calloc(1, sizeof(mcro_node));
}

char * get_mcro_body(mcro_node * mcro)
{
    return (mcro->body);
}

mcro_node *find_mcro(mcro_node *head, char *name)
{
    while (head)
    {
        if (!strcmp(head->name, name))
            return head;
        head = head->next;
    }
    return NULL;
}


void add_mcro(mcro_node **head, char *name, char *body)
{
    mcro_node *new = new_mcro();
    strcpy(new->name, name);
    new->body = body;
    new->next = *head;
    *head = new;
}
void free_mcro_list(mcro_node *head)
{

    while(head)
    {
        mcro_node *tmp = head;
        head = head->next;
        free(tmp->body);
        free(tmp);

    }
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

#ifndef mcro_DICT_H
#define mcro_DICT_H

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define NUM_OF_LETTERS 26
#define MAX_SYMBOL 31

typedef struct mcro_node mcro_node;
typedef struct mcro_dict mcro_dict;

mcro_node *new_mcro();
mcro_dict *new_dict();
void add_mcro(mcro_node **head, char *name, char *body);
void insert_mcro_dict(mcro_dict *mcros, char *name, char *body);
mcro_node *find_mcro(mcro_node *head, char *name);
mcro_node *find_mcro_dict(mcro_dict *mcros, char *name);

char * get_mcro_body(mcro_node * mcro);

void free_mcro_list(mcro_node *head);
void free_dictionary(mcro_dict *mcros);

void *validate_calloc(size_t nitems, size_t size);

#endif

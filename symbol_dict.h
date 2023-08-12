#ifndef SYMBOL_DICT_H
#define SYMBOL_DICT_H

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define NUM_OF_LETTERS 26


#define MAX_SYMBOL 31

typedef enum boolean {
    TRUE,
    FALSE
} boolean;

typedef struct symbol_dict symbol_dict;
typedef struct symbol_node symbol_node;


symbol_node *new_symbol();
symbol_node *find_symbol(symbol_node *head, char *name);
symbol_node *get_next_symbol(symbol_node *symbol);
int get_is_data(symbol_node *symbol);
int get_is_extern(symbol_node *symbol);
int get_line_number(symbol_node *symbol);
char *get_name(symbol_node *symbol);
int get_is_entry(symbol_node *symbol);
int get_address(symbol_node *symbol);

void set_is_entry(symbol_node *symbol,int is_entry);
void set_is_data(symbol_node *symbol,int is_data);
void set_address(symbol_node *symbol,int address);

void add_symbol(symbol_node **head, char *name, int address, int line_number, int is_extern, int is_entry,int is_data);
void *validate_calloc_symbol(size_t nitems, size_t size);
void free_symbol_list(symbol_node *head);

symbol_dict *new_symbol_dict();
symbol_node *get_page(symbol_dict *dict,int page_number);
void insert(symbol_dict *symbols, char *name, int address, int line_number, int is_extern, int is_entry, int is_data);
symbol_node *find(symbol_dict *symbols, char *name);
void free_dictionary_symbol(symbol_dict *symbols);

#endif

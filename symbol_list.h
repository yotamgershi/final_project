#ifndef SYMBOL_LIST_H
#define SYMBOL_LIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_SYMBOL 31

typedef enum boolean {
    TRUE,
    FALSE
} boolean;

/**
 * @struct symbol_node
 * @brief A node representing a symbol in the symbol list.
 */
typedef struct symbol_node {
    struct symbol_node *next; /**< Pointer to the next symbol in the list. */
    char name[MAX_SYMBOL]; /**< The name of the symbol. */
    int address; /**< The address associated with the symbol. */
    int line;    /**< The line in "am" file of the symbol. */
    int is_extern; /**< Flag indicating whether the symbol is a extern symbol (TRUE) or not (FALSE). */
    int is_entry; /**< Flag indicating whether the symbol is an entry symbol (TRUE) or not (FALSE). */
    int is_data; /**< Flag indicating whether the symbol is a data symbol (TRUE) or not (FALSE). */
} symbol_node;

/**
 * @brief Creates a new symbol node and initializes it.
 *
 * This function allocates memory for a new symbol node, initializes its fields, and returns a pointer to the node.
 *
 * @return A pointer to the newly created symbol node.
 */
symbol_node *new_symbol();

/**
 * @brief Finds a symbol in the symbol list by its name.
 *
 * This function searches for a symbol with the given name in the symbol list and returns a pointer to the node if found,
 * or NULL if the symbol is not present in the list.
 *
 * @param head The head of the symbol list.
 * @param name The name of the symbol to find.
 * @return A pointer to the symbol node if found, or NULL if not found.
 */
symbol_node *find_symbol(symbol_node *head, char *name);

/**
 * @brief Adds a new symbol to the symbol list.
 *
 * This function creates a new symbol node with the provided information and adds it to the symbol list.
 *
 * @param head A pointer to the head of the symbol list.
 * @param name The name of the symbol to add.
 * @param address The address associated with the symbol.
 * @param is_data A boolean value indicating whether the symbol is a extern symbol (TRUE) or not (FALSE).
 * @param is_entry A boolean value indicating whether the symbol is an entry symbol (TRUE) or not (FALSE).
 * @param is_data A boolean value indicating whether the symbol is a data symbol (TRUE) or not (FALSE).
 */
void add_symbol(symbol_node **head, char *name, int address, int line, int is_extern, int is_entry,int is_data);

/**
 * @brief Allocates memory for an array of elements, initialized to zero.
 *
 * This function is similar to the standard calloc() function but also checks if the memory allocation is successful.
 *
 * @param nitems The number of elements to allocate.
 * @param size The size of each element in bytes.
 * @return A pointer to the allocated memory, or NULL if allocation fails.
 */
void *validate_calloc_symbol(size_t nitems, size_t size);

/**
 * @brief Allocates memory for a single element, initialized to zero.
 *
 * This function is similar to the standard malloc() function but also checks if the memory allocation is successful.
 *
 * @param size The size of the element in bytes.
 * @return A pointer to the allocated memory, or NULL if allocation fails.
 */
void *validate_malloc_symbol(size_t size);

/**
 * @brief Frees the memory occupied by the symbol list.
 *
 * This function frees the memory occupied by the entire symbol list, including all symbol nodes.
 *
 * @param head The head of the symbol list to free.
 */
void free_symbol_list(symbol_node *head);

#endif

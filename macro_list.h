#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_MACRO 31

/**
 * @struct macro_node
 * @brief A node representing a macro definition in the macro list.
 */
typedef struct macro_node {
    struct macro_node *next; /**< Pointer to the next macro node in the list. */
    char name[MAX_MACRO]; /**< The name of the macro. */
    char *def; /**< The definition of the macro. */
} macro_node;

/**
 * @brief Creates a new macro node and initializes it.
 *
 * This function allocates memory for a new macro node, initializes its fields, and returns a pointer to the node.
 *
 * @return A pointer to the newly created macro node.
 */
macro_node *new_macro();

/**
 * @brief Finds a macro definition in the macro list by its name.
 *
 * This function searches for a macro with the given name in the macro list and returns a pointer to the node if found,
 * or NULL if the macro is not present in the list.
 *
 * @param head The head of the macro list.
 * @param name The name of the macro to find.
 * @return A pointer to the macro node if found, or NULL if not found.
 */
macro_node *find_macro(macro_node *head, char *name);

/**
 * @brief Adds a new macro definition to the macro list.
 *
 * This function creates a new macro node with the provided information and adds it to the macro list.
 *
 * @param head A pointer to the head of the macro list.
 * @param name The name of the macro to add.
 * @param def The definition of the macro to add.
 */
void add_macro(macro_node **head, char *name, char *def);

/**
 * @brief Allocates memory for an array of elements, initialized to zero.
 *
 * This function is similar to the standard calloc() function but also checks if the memory allocation is successful.
 *
 * @param nitems The number of elements to allocate.
 * @param size The size of each element in bytes.
 * @return A pointer to the allocated memory, or NULL if allocation fails.
 */
void *validate_calloc(size_t nitems, size_t size);

/**
 * @brief Allocates memory for a single element, initialized to zero.
 *
 * This function is similar to the standard malloc() function but also checks if the memory allocation is successful.
 *
 * @param size The size of the element in bytes.
 * @return A pointer to the allocated memory, or NULL if allocation fails.
 */
void *validate_malloc(size_t size);

/**
 * @brief Frees the memory occupied by the macro list.
 *
 * This function frees the memory occupied by the entire macro list, including all macro nodes and their definitions.
 *
 * @param head The head of the macro list to free.
 */
void free_list(macro_node *head);

#endif

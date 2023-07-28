#ifndef MACRO_DICT_H
#define MACRO_DICT_H

#include "macro_list.h"
#include <ctype.h>
#define NUM_OF_LETTERS 26

/**
 * @struct macro_dict
 * @brief A dictionary to store macro definitions using a hash table.
 *
 * This structure represents a dictionary to store macro definitions using a simple hash table.
 * The hash table is implemented as an array of macro_node pointers, where each index represents a hash bucket.
 */
typedef struct macro_dict {
    macro_node *dict[NUM_OF_LETTERS]; /**< The hash table array of macro_node pointers (hash buckets). */
} macro_dict;

/**
 * @brief Inserts a new macro definition into the macro dictionary.
 *
 * This function inserts a new macro definition into the macro dictionary.
 * It calculates the hash value for the macro name to determine the appropriate hash bucket in the hash table.
 * If the macro with the given name already exists in the dictionary, the new definition is appended to the existing macro.
 * If the macro does not exist, a new macro node is created and added to the corresponding hash bucket.
 *
 * @param macros A pointer to the macro dictionary.
 * @param name The name of the macro to insert.
 * @param body The body of the macro to insert.
 */
void insert_macro_dict(macro_dict *macros, char *name, char *body);

/**
 * @brief Finds a macro definition in the macro dictionary by its name.
 *
 * This function searches for a macro with the given name in the macro dictionary and returns a pointer to the macro node if found,
 * or NULL if the macro is not present in the dictionary.
 * It calculates the hash value for the macro name to determine the hash bucket to search in the hash table.
 *
 * @param macros A pointer to the macro dictionary.
 * @param name The name of the macro to find.
 * @return A pointer to the macro node if found, or NULL if not found.
 */
macro_node *find_macro_dict(macro_dict *macros, char *name);
void free_dictionary(macro_dict *macros);

#endif

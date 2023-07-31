#ifndef SYMBOL_DICT_H
#define SYMBOL_DICT_H

#include "symbol_list.h"
#include <ctype.h>

/**
 * @struct symbol_dict
 * @brief A dictionary to store symbols using a hash table.
 *
 * This structure represents a dictionary to store symbols using a simple hash table.
 * The hash table is implemented as an array of symbol_node pointers, where each index represents a hash bucket.
 */
typedef struct symbol_dict {
    symbol_node *dict[26]; /**< The hash table array of symbol_node pointers (hash buckets). */
} symbol_dict;


void insert(symbol_dict *symbols, char *name, int address, int line, boolean is_extern, boolean is_entry);


/**
 * @brief Finds a symbol in the symbol dictionary by its name.
 *
 * This function searches for a symbol with the given name in the symbol dictionary and returns a pointer to the symbol node if found,
 * or NULL if the symbol is not present in the dictionary.
 * It calculates the hash value for the symbol name to determine the hash bucket to search in the hash table.
 *
 * @param symbols A pointer to the symbol dictionary.
 * @param name The name of the symbol to find.
 * @return A pointer to the symbol node if found, or NULL if not found.
 */
symbol_node *find(symbol_dict *symbols, char *name);

#endif

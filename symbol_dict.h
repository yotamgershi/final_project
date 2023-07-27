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

/**
 * @brief Inserts a new symbol into the symbol dictionary.
 *
 * This function inserts a new symbol into the symbol dictionary.
 * It calculates the hash value for the symbol name to determine the appropriate hash bucket in the hash table.
 * If the symbol with the given name already exists in the dictionary, the new definition is appended to the existing symbol.
 * If the symbol does not exist, a new symbol node is created and added to the corresponding hash bucket.
 *
 * @param symbols A pointer to the symbol dictionary.
 * @param name The name of the symbol to insert.
 * @param def The definition of the symbol to insert.
 */
void insert(symbol_dict *symbols, char *name, char *def);

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

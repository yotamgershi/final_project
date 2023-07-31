#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_dict.h"

#define MAX_LINE 82
#define DELIMITER " \n\t"

/**
 * @brief Pre-assemble an input assembly file and generate an output file.
 *
 * This function processes an input assembly file (with a .as extension) and generates an output file with a .am extension.
 * It performs pre-assembly tasks such as handling macros and resolving references to them.
 *
 * @param file_name The name of the input assembly file.
 * @return A pointer to the generated output file, or NULL if an error occurs.
 */
FILE *pre_assembler(char *file_name);

/**
 * @brief Open a file in a specified mode, validating the file opening.
 *
 * This function opens a file in the specified mode and validates the file opening process.
 *
 * @param file_name The name of the file to open.
 * @param mode The mode in which to open the file (e.g., "r" for read, "w+" for read/write).
 * @return A pointer to the opened file, or NULL if the file could not be opened.
 */
FILE *validate_fopen(const char *file_name, char *mode);

/**
 * @brief Add a suffix to a given file name.
 *
 * This function appends a suffix to the given file name and returns the new file name with the suffix.
 *
 * @param file_name The original file name.
 * @param suffix The suffix to append to the file name.
 * @return The new file name with the appended suffix.
 */
char *add_suffix(char *file_name, char *suffix);

/**
 * @brief Calculate the length of a macro definition in the input file.
 *
 * This function calculates the length of a macro definition in the input file, starting from the given position.
 * The length includes the number of characters until the "endmcro" directive is encountered.
 *
 * @param input_file The input file to read from.
 * @param start_pos The starting position to begin calculating the length.
 * @param the curenet line

 * @return The length of the macro definition in bytes.
 */
size_t calculate_macro_length(FILE *input_file, fpos_t start_pos,char line[]);
/**
 * @brief cheak if a given string is instruction word
 *
 * 
 * @param str The to cheak 
 * @return true(1) or false(0) as needed 
 */
int is_instruction(char * str);
/**
 * @brief cheak if we have corect definition of mcro
 *
 * 
 * @param line the line where the mcro declerd
 * @return true(1) or false(0) as needed 
 */
int valid_macro_def(char * line);

/**
 * @brief A boolean data type representing true or false values.
 */
void print_long_line(FILE * input_file, FILE * dest_file,char *line);
typedef enum {
    false,
    true
} bool;

#endif

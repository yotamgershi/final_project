/**
 * @file error.h
 * @brief Header file for error handling in the assembler process.
 */

#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "globals.h"

#define NUM_OF_RESERVED_WORDS 29
#define MAX_LEN_OF_RESERVED_WORD 31
#define NUM_OF_COMMANDS 16
#define ERROR_MSG_SIZE 256
#define SPACE_AND_COMMA " ,\t\n"
#define SPACE " \t"
#define MAX_LINE 82
/* -------------------------------------------------------- Enums --------------------------------------------------------*/


/* numeration to represent different error types. */
typedef enum 
{
    LABEL_RESERVED_WORD,
    LABEL_ALREADY_DEFINED,
    ERROR_THREE,
    ERROR_FOUR,
    ERROR_FIVE,
    WRONG_ADDRESSING_TYPE,
    NUM_ERRORS
} error_code;

/*-------------------------------------------------------- Structs --------------------------------------------------------*/

typedef struct 
{
    char *cmd, *src_type, *dest_type; 
} lookup_table;


typedef struct 
{
    error_code code;
    const char *message; 
} error_item;

extern error_item errors[NUM_ERRORS];

extern char reserved_words[NUM_OF_RESERVED_WORDS][MAX_LEN_OF_RESERVED_WORD];


/*-------------------------------------------------------- Functions --------------------------------------------------------*/

/* line */
bool is_valid_commas(char *operands); /* works fine */
char *remove_spaces(char *line); /* works fine */
void skip_spaces(char *str); /* works fine */
bool is_empty_line(char *line); /* works fine */
bool is_comment_line(char *line); /* works fine */
int count_words(char *line); /* works fine */


/* Labels*/
bool is_reseved_word(char *label); /* Works fine */
bool is_valid_label(char *label); /* Works fine */

/* operands */
char *find_address_type(char *operand);  /* works fine */
bool is_valid_operand_amount(char *line); /* TODO: check with Maor */
int cmd_index(char *cmd); /* Works fine */
bool is_valid_operand(char *operand); /* Works fine */
bool is_valid_dest_type(char *cmd, char *dest); /* TODO: check with Maor*/
bool is_valid_src_type(char *cmd, char *src); /* TODO: check with Maor*/


/* .string */
bool is_valid_string(char *str); /* Works fine */
bool is_string_directive (char *line) /* Works fine*/

/* .data */
bool is_valid_data_operand(char *operand);


/* .extern */
bool extern_valid_operand_amount();
bool extern_not_defined_operand(); /* also check if defined as entry */


/* .entry */
bool entry_valid_operand_amount();

/* int */
bool is_legal_int(); /* add a test if its in range -511 to 512 for most, and -2047 to 2048 for data */

#endif /* ERROR_H */

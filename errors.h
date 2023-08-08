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
bool is_empty_line(char *line); /* works fine */
bool is_comment_line(char *line);

/* Labels*/
bool is_reseved_word(char *label); /* Works fine */
bool is_valid_label(char *label); /* Works fine */
bool is_valid_address_type(); /*  */


/* operands */
char *find_address_type(char *operand);
bool is_valid_address_type(char *cmd, char *operand);
bool is_valid_operand_amount(char *line); /* for each cmd type: 1/2/3 operands */
int cmd_index(char *cmd);
bool is_valid_operand();

/* .string */
bool is_valid_string(char *str);

/* .data */
bool is_valid_data_operand(char *operand);


/* .extern */
bool extern_valid_operand_amount();
bool extern_not_defined_operand(); /* also check if defined as entry */


/* .entry */
bool entry_valid_operand_amount();

/* int */
bool is_legal_int(); /* add a test if its in range -511 to 512 for most, and -2047 to 2048 for data */

/*-------------------- Helper Functions --------------------*/

bool has_two_consecutive_commas(char *line);
char *remove_spaces(char *line);
char *addressing_type(char *operand);
void skip_spaces(char *str);
int count_words(char *line);


#endif /* ERROR_H */

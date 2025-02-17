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
    SUCCESS,
    INVALID_COMMAS,
    RESERVED_WORD,
    INVALID_LABEL,
    INVALID_OPERAND_TYPE,
    INVALID_OPERAND_AMOUNT,
    INVALID_OPERAND,
    INVALID_STRING,
    INVALID_DATA,
    INVALID_EXTERN,
    INVALID_EXTERN_OPERAND,
    INVALID_ENTRY,
    INVALID_ENTRY_OPERAND,
    NUM_ERRORS
} error_code;

/*-------------------------------------------------------- Structs --------------------------------------------------------*/

typedef struct 
{
    char *cmd, *src_type, *dest_type;
    int num_of_operands; 
} lookup_table;


typedef struct 
{
    error_code code;
    const char *message; 
} error_item;

extern error_item errors[NUM_ERRORS];

extern char reserved_words[NUM_OF_RESERVED_WORDS][MAX_LEN_OF_RESERVED_WORD];


/*-------------------------------------------------------- Functions --------------------------------------------------------*/

bool validate_line(char *line, int line_number);
bool handle_error(int line_number, int error_index);


/* line */
bool is_valid_commas(char *operands); /* add test if there are enough commas and they are placed right */
char *remove_spaces(char *line);
void skip_spaces(char *str);
bool is_empty_line(char *line); 
bool is_comment_line(char *line); 
int count_words(char *line); 
bool is_empty_or_comment(char *line);

/* Labels*/
bool is_reseved_word(char *label);
bool is_cmd(char *word);
bool is_valid_label(char *label);
bool is_label_directive(char *label);

/* operands */
char *find_address_type(char *operand);  
bool is_valid_type(char *cmd, char *dest, bool is_src); 
char *valid_address_type(int i, bool is_src); 
int valid_operand_amount(char *cmd, char *operands);
int cmd_index(char *cmd); 
bool is_valid_operand(char *operand); 
error_code is_valid_cmd_operands(char *cmd, char *operands);
bool is_valid_register(char *reg);
bool is_sign(char digit);

/* .string */
bool is_valid_string(char *str); 
bool is_string_directive (char *cmd);
bool two_quotes(char *str);

/* .data */
bool is_data_directive(char *cmd); 
bool is_valid_data(char *operand); 

/* .extern */
bool is_valid_extern(char *line); 
bool is_valid_extern_operands(char *operands); 
bool is_extern_directive(char *cmd); 
bool is_valid_label_for_extern_and_entry(char *label); 

/* .entry */
bool is_valid_entry(char *line); 
bool is_valid_entry_operands(char *operands); 
bool is_entry_directive(char *cmd); 
bool entry_valid_operand_amount(char *line); 

#endif /* ERROR_H */

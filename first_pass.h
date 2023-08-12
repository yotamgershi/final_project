#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "symbol_dict.h"
#include "pre_assembler.h"
#define KS 100 /* kept space*/

/* gets am file, pointers to DC,IC,error_first_pass. update DC/IC to number of data/instruction words in file,
   build symbol table, and return it. */
symbol_dict *first_pass(FILE * pre_assembled_file,int *DC,int *IC,int *error_first_pass);

/*receive line, its number, and file pointer. cheak if the length is valid if so return true(1), otherwise return false(0)
and, countinu to read file until enter to get to next line. of course print error message in case of error. */
int is_valid_length(char *line,int line_number,FILE *pre_assembled_file);

/*receive line, cheak if it's an empty or comment line if so return true(1), otherwise return false(0)
  we allow white spaces before ; in comment line */
int is_empty_or_comment_line(char *line);

/*receive line cheak if first or second word is ".extern" if either return true(1) else false(1)
  because if line start wite extern or there's (unused!!) label in the begining, and then extern, its an extern line */
int is_extern(char * line);

/*receive line cheak if first or second word is ".entry" if either return true(1) else false(1)
  because if line start wite entry or there's (unused!!) label in the begining, and then entry, its an entry line */
int is_entry(char * line);

/* receive line that has lable, pointer to lable_name, and a "flag" if the line is extern.
   update the lable_name to contain the name of the symbol declerd/defined in the line, unless
   its extern line, in which case it's update the name to be ".extern" because extern line could decler on number
   of lable unlike an entry declartion or definition of lable*/
void get_label_name(char * line,char * name,int externl);

/*receive node of symbole that had been declered/defined already in the file. cheak if the current line is not contradicting the former one.
  assuming that multipul declartion of entry/extern with the same name is ok, but gives a warning. */
void is_repeat_def(symbol_node *current_nude,int line_number,int externl,int internal,int *error_first_pass,int DC, int IC, int dc);

/* receive line return true(1) if the line decler or define a label, else return false(0)*/
int is_label_line(char *line);

/* receive dictionary of symbol, line that declers one or more extrn labels, the line number, and pointer to signel if there are errors
   it adds to the dictionary the labels that are declers in this line*/
void insert_extern(symbol_dict *symbol_table,char *line,int line_number,int *error_first_pass);

/*count the number of data words needed to be encod for this line, return this number */
int count_data(char *line);

/*count the number of instructions words needed to be encod for this line, return this number */
int count_instructions(char *line);

#endif

#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "symbol_dict.h"
#include "pre_assembler.h"
#include "errors.h"


symbol_dict *first_pass(FILE * pre_assembled_file,int *DC,int *IC,int *error_first_pass);
int validate_line(char *line, int line_number);

int is_valid_length(char *line,int line_number,FILE *pre_assembled_file);

int is_extern(char * line);
int is_entry(char * line);
void get_label_name(char * line,char * name,int internal);
void is_repeat_def(symbol_node *current_nude,int line_number,int externl,int internal,int *error_first_pass,int DC, int IC, int dc);
int is_label_line(char *line);
void insert_extern(symbol_node *symbol_table,char *line,int line_number,int *error_first_pass);
int count_data(char *line);
int count_instructions(char *line);

#endif

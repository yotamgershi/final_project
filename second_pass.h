#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "symbol_dict.h"
#include "first_pass.h"
#define MAX_LINE 82
#define true 1
#define false 0
#define BiW 12 /* number of bits in word*/
#define MAX_FILE_SIZE 924 /* maximum number of words to encode in single file */
#define KS 100 /* kept space*/


void second_pass(char *file_name,int DC,int IC,symbol_dict *symbol_table);

void sets_files_name(char *file_name,char *am_file_name,char *ext_file_name,char *ent_file_name,char *ob_file_name);

void update_data_address(symbol_dict *symbol_table,int IC);
void encode_data_line(char data_image[][BiW],char *line,int *data_index);
void encode_data(char *word,int data);
void encode_instruction_line(char instruc_image[][BiW],char *line,int *instructions_index,int line_number,FILE *ext_file,int *error,symbol_dict *symbol_table);
void encode_instruction_opcode(char *word,int command_num);
void encode_sourse_oprend(char *word,char *first_oprend);
void encode_destantion_oprend(char *word,char *second_oprend);
void encode_first_rej(char *word,int rejster_num);
void encode_second_rej(char *word,int rejster_num, int is_first_also_rej);
void encode_label(char *word,char *label_name,int line_number,FILE *ext_file,int *error,symbol_dict *symbol_table,int index);
void print_entry_to_file(FILE *ent_file,char *ent_file_name,symbol_dict *symbol_table,int *error);
void print_tables_to_file(char instruc_image[][BiW],char data_image[][BiW],int DC,int IC,FILE *ob_file);
void print_binary_word_64(char *word,FILE *ob_file);


#endif

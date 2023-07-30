#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "symbol_dict.h"
#include "pre_assembler.h"
#include "errors.h"

symbol_dict *first_pass(pre_assembled_file); /* TODO: change FILE *first_pass_file to dict *symbol_table */
boolean validate_line(char *line);

#endif
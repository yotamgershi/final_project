#include "errors.h"


error_item errors[NUM_ERRORS] = {
    {ERROR_ONE, "Error One Message"},
    {ERROR_TWO, "Error Two Message"},
    {ERROR_THREE, "Error Three Message"},
    {ERROR_FOUR, "Error Four Message"},
    {ERROR_FIVE, "Error Five Message"}
};

void init_error_log(error_log *log)
{
    log->pre_assembler_errors.step = PRE_ASSEMBLER;
    log->pre_assembler_errors.head = NULL;

    log->first_pass_errors.step = FIRST_PASS;
    log->first_pass_errors.head = NULL;

    log->second_pass_errors.step = SECOND_PASS;
    log->second_pass_errors.head = NULL;
}

void add_error(error_log *log, assembler_step step, error_code code, int line_number)
{
    
}

void print_errors(error_list *error_list);

void print_all_errors(error_log *log);

void clear_error_log(error_log *log);


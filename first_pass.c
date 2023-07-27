#include "first_pass.h"

symbol_dict *first_pass(pre_assembled_file) /* TODO: change FILE *first_pass_file to dict *symbol_table */
{
    char line[MAX_LINE];
    while(fgets(line, MAX_LINE, pre_assembled_file));
        if (validate_line(line))
            /* do */
    return NULL;
}

boolean validate_line(char *line, int line_number)
{
    int cmd_index;
    char copy_line[MAX_LINE], *token;
    strcpy(copy_line, line);
    token = strtok(copy_line, DELIMITER);
    if (is_empty_or_comment_line(token))
        return true;
    if (is_label(token))
    {   
        if (!is_valid_label(token))
            return false;
        token = strtok(NULL, DELIMITER);
        if (is_entry_or_extern(token))
            warning(line_number);
    }    
    if (!(cmd_index = is_reserved_word(token)))
        return false;
    /* LABEL: cmd op1, op2*/
    /*op1, op2*/
    /* TODO: check the op amount, check commas, check op address_type or if data check op = number*/
    if (valid_op_amount(cmd_index, op_amount))
}

boolean is_empty_or_comment_line(char* first_word)
{
    if (!first_word)
        return true;
    if(first_word[0] == ';')
        return true;
    return false;
}

void warning(int line_number)
{

}

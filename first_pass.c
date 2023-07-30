#include "first_pass.h"

symbol_dict *first_pass(pre_assembled_file) /* TODO: change FILE *first_pass_file to dict *symbol_table */
{
    int line_number = 0;
    char line[MAX_LINE];
    while(fgets(line, MAX_LINE, pre_assembled_file));
        if (validate_line(line, line_number))
            /* do */
        line_number++;
    return NULL;
}

boolean validate_line(char *line, int line_number)
{
    int cmd_index, op_amount;
    char copy_line[MAX_LINE], *token;
    strcpy(copy_line, line);
    token = strtok(copy_line, DELIMITER);
    if (!is_length_valid)
        return false;
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
        {
            int ops; /* TODO: change ops to the operands of the cmd*/
            valid_op_types(ops);
        }
    if (!no_multipul_commas(line))
    {
        return false;
    }
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
    printf("");
}

int valid_op_amount(char *line)
{
    int count = 0;
    char *ptr = line;
    int comma_encountered = 0;

    while (*ptr != '\n' && *ptr != '\0') {
        if (*ptr == ',') {
            comma_encountered = 1;
        } else if (*ptr != ' ' && comma_encountered) {
            count++;
            comma_encountered = 0;
        }
        ptr++;
    }

    return count;
}

bool has_valid_commas(const char *line) 
{
    bool last_was_comma = false;

    while (*line) 
    {
        if (*line == ',') 
        {
            if (last_was_comma) 
                return false; 
            last_was_comma = true;
        } 
        else if (*line != ' ') 
            last_was_comma = false;
        line++;
    }

    return true; // No multiple commas found
}

bool is_length_valid(char *line)
{
    if (strlen(line) < 81)
        return true;
    if (strlen(line) == 81 && line[81]!='/n')
        return false;
}
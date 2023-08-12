#include "errors.h"

void handle_error(int line_number, int error_index)
{
    printf("in line %d: %s.", line_number, errors[error_index].message);
}

lookup_table cmds[] = 
{
    {"mov", "135", "35"},
    {"cmp", "135", "135"},
    {"add", "135", "35"},
    {"sub", "135", "35"},
    {"not", "", "35"},
    {"clr", "", "35"},
    {"lea", "3", "35"},
    {"inc", "", "35"},
    {"dec", "", "35"},
    {"jmp", "", "35"},
    {"bne","" , "35"},
    {"red","" , "35"},
    {"prn", "", "135"},
    {"jsr","" ,"35"},
    {"rts","" ,"" },
    {"stop","" ,"" }
};

error_item errors[NUM_ERRORS] = {
    {INVALID_COMMAS, "Commas are not valid"},
    {RESERVED_WORD, "The word is not familier"},
    {INVALID_LABEL, "Label is invalid"},
    {INVALID_OPERAND_TYPE, "Invalid operand type for command"},
    {INVALID_OPERAND_AMOUNT, "Invalid operand amountfor command"},
    {INVALID_OPERAND, "Invalid operand for command"},
    {INVALID_STRING, "Invalid string definition"},
    {INVALID_DATA, "Invalid data definition"},
    {INVALID_EXTERN, "Invalid extern definition"},
    {INVALID_EXTERN_OPERAND, "Invalid extern operand"},
    {INVALID_ENTRY, "Invalid entry usage"},
    {INVALID_ENTRY_OPERAND, "Invalid entry operand"}    
};

char reserved_words[NUM_OF_RESERVED_WORDS][MAX_LEN_OF_RESERVED_WORD] = 
{
    "@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7",
    "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop",
    "label", ".data", ".string", ".entry", ".extern"
};
/* -------------------------------------------------------- validate_line (main function) -------------------------------------------------------- */

bool validate_line()
{
    /*
        check if empty or comment
        check if first word is reserved word
            if yes - check if command is valid: instruction / entry / extern
                if yes - validate the line
                if no - handle error and return false 
            if no - check if first word is legit lable
                if no - handle error and return false
                if yes - check if the rest of the line is valid
                    if no - handle error and return false
    */
    return false;
}

/* -------------------------------------------------------- line -------------------------------------------------------- */

bool is_empty_line(char *line)
{
    char *line_no_spaces;

    if (!line)
        return false;
    
    line_no_spaces = remove_spaces(line);

    if (*line_no_spaces == '\0') {
        free(line_no_spaces);
        return true;
    }

    free(line_no_spaces);
    return false;
}

bool is_valid_commas(char *operands) /* TODO: this function is wrong, it returns (null) */
{
    bool result = true;
    char *line_no_spaces = remove_spaces(operands);

    if (strstr(line_no_spaces, ",,") || line_no_spaces[0] == ',' 
        || line_no_spaces[strlen(line_no_spaces) - 1] == ',')
    {
        result = false;
    }

    free(line_no_spaces);
    return result;
}

char *remove_spaces(char *line)
{
    char *copy_line = (char *)malloc(strlen(line) + 1);
    char *result = copy_line;
    strcpy(copy_line, line);

    while (*line != '\0')
    {
        if (!isspace(*line))
        {
            *copy_line++ = *line;
        }
        line++;
    }

    *copy_line = '\0';
    return result;
}

bool is_comment_line(char *line)
{
    char *line_no_spaces = remove_spaces(line);

    if (*line_no_spaces == ';')
    {
        free(line_no_spaces);
        return true;
    }
        
    free(line_no_spaces);
    return (*line == ';');
}


/* -------------------------------------------------------- Labels -------------------------------------------------------- */
bool is_reseved_word(char *label)
{
    int i;
    for(i = 0; i < NUM_OF_RESERVED_WORDS; i++)
    {
        if (!strcmp(label, reserved_words[i]))
            return true;
    }
    return false;
} 

bool is_valid_label(char *label)
{
    int i, len;
    if (label == NULL || strlen(label) > 31)
        return false;
    len = strlen(label);
    if (!isalpha(label[0]))
        return false;
    for (i = 1; i < len - 1; i++)
    {
        if (!isalnum(label[i])) 
            return false;
    }
    if (label[len - 1] != ':')
        return false;
    return true;
}


/* -------------------------------------------------------- operands --------------------------------------------------------*/

bool is_valid_type(char *cmd, char *dest, bool is_src)
{
    int i;
    char *op_address_type, *addressing_type;
    for (i = 0; i <= 15; i++)
    {
        if (!strcmp(cmds[i].cmd, cmd))
            op_address_type = valid_address_type(i, is_src);
    }
    if ((addressing_type = find_address_type(dest)) == NULL)
        return false;
    if (strstr(op_address_type, addressing_type))
        return true;
    return false;
}

char *valid_address_type(int i, bool is_src)
{
    return is_src ? cmds[i].src_type : cmds[i].dest_type;
}

char *find_address_type(char *operand)
{
    if (isdigit(operand[0]))
        return "1";
    if (isalpha(operand[0])) 
        return "3";
    if (operand[0] == '@')
                return "5";
    return NULL;
}

bool is_sign(char digit)
{
    return (digit == '-' || digit == '+');
}

bool is_valid_number_operand(char *operand)
{
    if (isdigit(*operand) || is_sign(*operand))
    {
        while (isdigit(*++operand))
        ;
        if (!*operand || isspace(*operand))
            return true;
    }

    return false;
}

is_valid_register(char *reg)
{
    return (reg[0] == '@' && reg[1] == 'r' && reg[2] >= '0' && reg[2] <= '7');
}

bool is_valid_operand(char *operand)
{
    if (is_valid_number_operand(operand))
        return true;
    
    if (is_valid_label_for_extern_and_entry(operand))
        return true;
    
    if (is_valid_register(operand))
        return true;

    return false;
}

bool is_valid_operand_amount(char *line)
{
    char copy_line[MAX_LINE], *first_word;
    int command_index, op_amount = count_words(line);


    strcpy(copy_line, line);
    first_word = strtok(copy_line, SPACE_AND_COMMA);

    if (!first_word)
        return true;
    command_index = cmd_index(first_word);
    if (command_index == EOF)
        return false;
    if (command_index <= 4)
        return(op_amount == 2);
    if (command_index <= 13)
        return(op_amount == 1);
    if (command_index <= 15)
        return(op_amount == 0);

    return false;
}

int cmd_index(char *cmd)
{
    int i;
    for (i = 0; i < NUM_OF_COMMANDS; i++)
    {
        if (!strcmp(cmd, cmds[i].cmd))
            return i;
    }
    return EOF;
}

int count_words(char *line)
{
    int word_count = 0;
    int in_word = 0;
    while (*line)
    {
        if ( isspace(*line) || *line == ',')
            in_word = 0;
        else if (!in_word)
        {
            in_word = 1;
            word_count++;
        }
        line++;
    }
    return word_count;
}

void skip_spaces(char *line) 
{
    char *dst = line;
    while (*line) 
    {
        if (*line != ' ') 
        {
            *dst = *line;
            dst++;
        }
        line++;
    }
    *dst = '\0';
}

/* .string */

bool is_string_directive(char *line)
{
    char copy_line[MAX_LINE];
    char *directive;

    strcpy(copy_line, line);
    directive = strtok(copy_line, SPACE);

    if (directive && !strcmp(directive, ".string"))
        return true;
    return false;
}

bool is_valid_string(char *str)
{
    char *first_quote = strchr(str, '"');
    char *last_quote = strrchr(str, '"');
    char *ptr;

    if (first_quote && last_quote && first_quote != last_quote) 
    {
        for (ptr = str; ptr < first_quote; ptr++) 
        {
            if (!isspace(*ptr))
                return false;
        }
        for (ptr = last_quote + 1; *ptr != '\0'; ptr++) 
        {
            if (!isspace(*ptr))
                return false;
        }

        return true;
    }

    return false;
}

/* -------------------------------------------------------- .data --------------------------------------------------------*/

bool is_data_directive(char *line)
{
    char copy_line[MAX_LINE];
    char *directive;

    strcpy(copy_line, line);
    directive = strtok(copy_line, SPACE);

    if (directive && !strcmp(directive, ".data"))
        return true;
    return false;
}

bool is_valid_data(char *data)
{
    int i;
    bool has_digit = false;

    for (i = 0; i < strlen(data); i++)
    {
        if (!isspace(data[i]))
        {
            if (isdigit(data[i]))
            {
                has_digit = true;
            }
            else if (data[i] != '+' && data[i] != '-')
            {
                if (!has_digit)
                    return false;
                else
                    has_digit = false;
            }
        }
    }

    return has_digit;
}

/*-------------------------------------------------------- .extern --------------------------------------------------------*/

bool is_valid_extern(char *line)
{
    char *operands = strchr(line, ' ');
    if (operands) 
    {
        operands++;
        return is_valid_extern_operands(operands);
    }
    return false;
}

bool is_valid_extern_operands(char *operands)
{
    char copy_operands[MAX_LINE], *operand;
    strcpy(copy_operands, operands);
    operand = strtok(copy_operands, ",");
    
    while (operand != NULL)
    {
        if (!is_valid_label_for_extern_and_entry(operand))
            return false;
        
        operand = strtok(NULL, ",");
    }
    
    return true;
}

bool is_extern_directive(char *line)
{
    char copy_line[MAX_LINE];
    char *directive;

    strcpy(copy_line, line);
    directive = strtok(copy_line, SPACE);

    if (directive && !strcmp(directive, ".extern"))
        return true;
    return false;
}

bool is_valid_label_for_extern_and_entry(char *label) /* is_valid_label does the same and ':', seperate it.*/
{
    int i, len;

    if (label == NULL || strlen(label) > 31)
        return false;

    len = strlen(label);
    if (!isalpha(label[0]))
        return false;

    for (i = 1; i < len; i++)
    {
        if (!isalnum(label[i])) 
            return false;
    }

    return true;
}

/*-------------------------------------------------------- .entry -------------------------------------------------------- */

bool is_valid_entry(char *line)
{
    char *operands = strchr(line, ' ');
    if (operands) 
    {
        operands++;
        return is_valid_extern_operands(operands);
    }
    return false;
}

bool is_valid_entry_operands(char *operands)
{
    char *operand = strtok(operands, ",");
    
    while (operand != NULL)
    {
        if (!is_valid_label_for_extern_and_entry(operand))
            return false;
        
        operand = strtok(NULL, ",");
    }
    
    return true;
}

bool is_entry_directive(char *line)
{
    char copy_line[MAX_LINE];
    char *directive;

    strcpy(copy_line, line);
    directive = strtok(copy_line, SPACE);

    if (directive && !strcmp(directive, ".entry"))
        return true;
    return false;
}

bool entry_valid_operand_amount(char *line) 
{
    return (count_words(line) == 2);
}




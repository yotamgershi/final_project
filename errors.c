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
    {LABEL_RESERVED_WORD, "Label can't be a reserved word"},
    {LABEL_ALREADY_DEFINED, "Label can't be redefined"},
    {ERROR_THREE, "Error Three Message"},
    {ERROR_FOUR, "Error Four Message"},
    {WRONG_ADDRESSING_TYPE, "Adressing type not supported by this command"}
};

char reserved_words[NUM_OF_RESERVED_WORDS][MAX_LEN_OF_RESERVED_WORD] = 
{
    "@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7",
    "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop",
    "label", ".data", ".string", ".entry", ".extern"
};


/* line */

bool is_empty_line(char *line)
{
    return false;
}

bool is_valid_commas(char *operands) /* TODO: this function is wrong, it returns (null) */
{
    bool result;
    char *line_no_spaces = remove_spaces(operands);
    if (strstr(line_no_spaces, ",,"))
        result = false;
    else if (line_no_spaces[0] == ',' || line_no_spaces[strlen(line_no_spaces) - 1] == ',')
        result = false;
    else 
        result = true;
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

/* Labels*/
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

/* operands */

bool is_valid_address_type(char *cmd, char *operand)
{
    char *addressing_type = find_address_type(operand);
    return false;
}

char *find_address_type(char *operand)
{
    if (atoi(operand) <= 512 || atoi(operand) >= -511) /* TODO: use is digit */
        return "1";
    if (operand[0] == '@' && operand[1] == 'r' && operand[2] >= '0' && operand[2] <= '7' 
        && (operand[3] == ',' || operand[3] == ' '))
                return "5";
    return NULL;
}

bool is_valid_operand_amount(char *line) /* works fine */ 
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
    if (command_index <= 8)
        return(op_amount == 2);
    if (command_index >= 9 && command_index <= 13)
        return(op_amount == 1);
    if (command_index >= 14)
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
            in_word = 0; /* we are not inside a word anymore */
        else if (!in_word)
        {
            in_word = 1; /* we are inside a new word */
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

bool is_valid_operand() 
{
    return false;
}

/* .string */


/* .extern */
bool extern_valid_operand_amount() 
{
    return false;
}

bool extern_not_defined_operand() 
{
    return false;
} /* also check if defined as entry/


/* .entry */
bool entry_valid_operand_amount() 
{
    return false;
}

/* int */
bool is_legal_int() 
{
    return false;
}
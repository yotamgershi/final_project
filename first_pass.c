#include "first_pass.h"

symbol_dict *first_pass(pre_assembled_file) /* TODO: change FILE *first_pass_file to dict *symbol_table */
{
    int line_number = 0,DC=0,IC=0; /* DC,IC will have to be poinnter from main*/
    int ic,dc; /* count current number instruction/data */
    char line[MAX_LINE];
    char lable_name[MAX_SYMBOL];
    int internal,externl;
    int error_first_pass=false;
    symbol_node *current_nude,*symbol_table;
    symbol_table=NULL;
    current_nude=NULL;
    while(fgets(line, MAX_LINE, pre_assembled_file));
        if (validate_line(line, line_number))
        {
            ic=count_instructions(line); /* NEET TO WRITE H */
            dc=count_data(line);        /* NEET TO WRITE H */
            if(is_lable(line))         /* not including lable befor entry/extern */
            {
		externl = is_extern(line);                            
		internal = is_entry(line);                           
                get_lable_name(line,lable_name,internal); /* lable name of external will be ".extern" so allways go to insert_extern */
		if((current_nude=find(symbol_table, lable_name)) == NULL) 
                {
                    if(!internal && !externl)
                        insert(symbol_table, lable_name, DC+IC, line_number,externl, internal);
		    else if(internal)
                        insert(symbol_table, lable_name, -1, line_number,externl, internal);
                    else /* externl */
                        insert_extern(symbol_table,line,line_number,&error_first_pass) /*could be number of lable in line */
                }
		else 
                    is_repeat_def(current_nude,line_number,externl,internal,&error_first_pass);	
            }
            IC+=ic;
	    DC+=dc;
        }
        else
            error_first_pass=true;
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

/* YOTAM FONCTIONS */

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







/* HILLEL FONCTIONS ALL FONCTION ASSUME LINE IS CORRECT */


int is_extern(char * line)
{

    char copy_line[MAX_LINE], *token;
    strcpy(copy_line, line);
    token = strtok(copy_line, " \n\t");
    if(strcmp(token, ".extern"))
        return false;
    return true;
}

int is_entry(char * line)
{

    char copy_line[MAX_LINE], *token;
    strcpy(copy_line, line);
    token = strtok(copy_line, " \n\t");
    if(strcmp(token, ".entry"))
        return false;
    return true;
}

void get_lable_name(char * line,char * name,int internal)
{
    char copy_line[82], *token;
    strcpy(copy_line, line);
    token = strtok(copy_line, " \n\t");
    if(internal)
        token = strtok(NULL, " \n\t");  /* moving to name from ".entry" */
    else
        token[strlen(token) -1] = '\0' ; /* deleting ":" */
    strcpy(name, token);
}

void is_repeat_def(symbol_node *current_nude,int line_number,int externl,int internal,int *error_first_pass)
{
    if( (current_nude->address) > 0) /* MAYBE TO ADD CHKE IF (current_nude->is_entry = true) */
    {
        if(internal)
            current_nude->is_entry = true;
        else
        {
            printf("ERROR in line: %d symbol: %s is already defined in line: %d", line_number, current_nude->name,current_nude->line);
            *error_first_pass=true;
        }
    }
    else if(externl && (current_nude->is_extern))
	printf("WAWNING in line: %d symbol: %s is already defined as extern in line: %d", line_number, current_nude->name,current_nude->line);
    else if(internal && (current_nude->is_entry))
	printf("WAWNING in line: %d symbol: %s is already defined as entry in line: %d", line_number, current_nude->name,current_nude->line);
    else if(internal && (current_nude->is_extern))
    {
        printf("ERROR in line: %d symbol: %s is already defined as extern in line: %d", line_number, current_nude->name,current_nude->line);
        *error_first_pass=true;
    }
    else
    {
        printf("ERROR in line: %d symbol: %s is already defined as entry in line: %d", line_number, current_nude->name,current_nude->line);
        *error_first_pass=true;
    }       
}

int is_lable(char *line)
{
    char copy_line[MAX_LINE], *token;
    strcpy(copy_line, line);
    token = strtok(copy_line, " \n\t");

    if(is_entry(line) || is_extern(line)) /* line has the form: ".entry " or ".entry " */
        return true;
    if(token[strlen(token) -1] != ':') /* line is not: "lable: " nor the above form, so no lable */
        return false;
    token = strtok(NULL, " \n\t");
    if(!strcmp(token, ".extern") || !strcmp(token, ".entry")) /* line is: "lable: .entry " or "lable: .extern "*/
        return false;
    return true;
}


void insert_extern(symbol_node *symbol_table,char *line,int line_number,int *error_first_pass)
{
    char copy_line[MAX_LINE], *lable_name;
    symbol_node *current_nude;

    strcpy(copy_line, line);
    lable_name = strtok(copy_line, " ,\n\t"); /* now lable_name = .extern */
    lable_name = strtok(NULL, " ,\n\t");     /* now lable_name = first lable in line */
    while(lable_name)
    {
        if((current_nude=find(symbol_table, lable_name)) == NULL)
            insert(symbol_table, lable_name, -1, line_number,true, false);
        else
            is_repeat_def(current_nude,line_number,externl,internal,error_first_pass);
        lable_name = strtok(NULL, " ,\n\t");
    }
}

























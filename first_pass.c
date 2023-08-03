#include "first_pass.h"

symbol_dict *first_pass(FILE * pre_assembled_file,int *DC,int *IC,int *error_first_pass)
{
    int line_number = 0; 
    int ic,dc; /* count current number instruction/data */
    char line[MAX_LINE];
    char label_name[MAX_SYMBOL];
    int internal,externl;
    symbol_node *current_nude;
    symbol_dict *symbol_table;

    symbol_table=NULL;
    current_nude=NULL;

    while(fgets(line, MAX_LINE, pre_assembled_file));
    {
        if(is_valid_length(line,line_number,pre_assembled_file))
        {
            if (validate_line(line, line_number))
            {
                ic=count_instructions(line);
                dc=count_data(line);        
                if(is_label_line(line))         /* not including label befor entry/extern */
                {
		    externl = is_extern(line);                            
		    internal = is_entry(line);                           
                    get_label_name(line,label_name,internal); /* label name of external will be ".extern" so allways go to insert_extern */
		    if((current_nude=find(symbol_table, label_name)) == NULL) 
                    {
                        if(!internal && !externl && dc)
                            insert(symbol_table, label_name, *DC+100, line_number,externl, internal,true);
                        else if(!internal && !externl && !dc)
                            insert(symbol_table, label_name,*IC+100, line_number,externl, internal,false);    
		        else if(internal)
                            insert(symbol_table, label_name, -1, line_number,externl, internal,false);
                        else /* externl */
                            insert_extern(symbol_table,line,line_number,error_first_pass); /*could be number of label in line */
                    }
		    else 
                        is_repeat_def(current_nude,line_number,externl,internal,error_first_pass,*DC,*IC,dc);	
                }
                *IC+=ic;
	        *DC+=dc;
            }
            else
                *error_first_pass=true;
        }
        else
            *error_first_pass=true;
        line_number++;       
    }
    return symbol_table;
}

int validate_line(char *line, int line_number)
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











int is_valid_length(char *line,int line_number,FILE *pre_assembled_file)
{
    if((strlen(line) ==  MAX_LINE -1) && (line[MAX_LINE -1] != '\n'))
    {
        while(strlen(line) ==  MAX_LINE -1)
        {
            if(line[MAX_LINE -1] != '\n') /*not the end of the line, cuntinue to read the line */
                fgets(line, MAX_LINE, pre_assembled_file);
            else
	       break;
        }
        printf("ERROR in line: %d. The line is too long", line_number);
        return false;
    }
    else
        return true;
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

void get_label_name(char * line,char * name,int internal)
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

void is_repeat_def(symbol_node *current_nude,int line_number,int externl,int internal,int *error_first_pass,int DC, int IC, int dc)
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
    else if(externl && (current_nude->is_entry))
    {
        printf("ERROR in line: %d symbol: %s is already defined as entry in line: %d", line_number, current_nude->name,current_nude->line);
        *error_first_pass=true;
    }
    else /* externl=internal=false */
    {
        if(current_nude->is_extern)
        {
            printf("ERROR in line: %d symbol: %s is already defined as extern in line: %d", line_number, current_nude->name,current_nude->line);
            *error_first_pass=true;
        }
        else /* current_nude->is_entry = true. NOW the definition of the address lable that defined already as entry*/
        {
            if(dc)
                current_nude->address=DC+100;
            else
                current_nude->address=IC+100;
            current_nude->is_data=dc;     
        }
    
    }       
}

int is_label_line(char *line)
{
    char copy_line[MAX_LINE], *token;
    strcpy(copy_line, line);
    token = strtok(copy_line, " \n\t");

    if(is_entry(line) || is_extern(line)) /* line has the form: ".entry " or ".entry " */
        return true;
    if(token[strlen(token) -1] != ':') /* line is not: "label: " nor the above form, so no lable */
        return false;
    token = strtok(NULL, " \n\t");
    if(!strcmp(token, ".extern") || !strcmp(token, ".entry")) /* line is: "label: .entry " or "lable: .extern "*/
        return false;
    return true;
}


void insert_extern(symbol_dict *symbol_table,char *line,int line_number,int *error_first_pass)
{
    char copy_line[MAX_LINE], *label_name;
    symbol_node *current_nude;

    strcpy(copy_line, line);
    label_name = strtok(copy_line, " ,\n\t"); /* now label_name = .extern */
    label_name = strtok(NULL, " ,\n\t");     /* now label_name = first label in line */
    while(label_name)
    {
        if((current_nude=find(symbol_table, label_name)) == NULL)
            insert(symbol_table, label_name, -1, line_number,true, false,false); /* extern=true,enternl=false,is_data=false*/
        else
            is_repeat_def(current_nude,line_number,true,false,error_first_pass,0,0,0); /* extern=true,enternl=false, and DC,IC,dc has no meaning so 0*/
        label_name = strtok(NULL, " ,\n\t");
    }
}


int count_data(char *line)
{
    
    int dc=0;
    int i=0; 
    char copy_line[MAX_LINE], *token;
    strcpy(copy_line, line);
    token = strtok(copy_line, " ,\n\t");
    
    if(token[strlen(token) -1] == ':') /* If token=lable: */
        token = strtok(NULL, " ,\n\t");
    if(!strcmp(token, ".data"))
    {
        token = strtok(NULL, " ,\n\t"); /* token=first intger  */
        while(token)
        {
            dc++;
            token = strtok(NULL, " ,\n\t"); /* token=next intger  */
        }
    }
    if(!strcmp(token, ".string"))
    {
         
         while(line[i++] != '"'); /* now line[i] = first char in the string */
         while(line[i++] != '"')
             dc++;
         dc++; /* for the /0 char */
    }

    return dc;
}



int count_instructions(char *line)
{
    int ic=0;
    int i=0; 
    char  instruction_table[16][5] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};
    char copy_line[MAX_LINE], *token;
    strcpy(copy_line, line);
    token = strtok(copy_line, " ,\n\t");
    if(token[strlen(token) -1] == ':') /* If token=label: */
        token = strtok(NULL, " ,\n\t");
    for(i=0; i<16; i++)
        if(!strcmp(token, instruction_table[i]))
        {
            ic++;
            token = strtok(NULL, " ,\n\t");
            if(token) /* if there is oprend */
            {
                if(token[0]=='@') /* token=rejster*/
                {
                    ic++;                          /* need word for the rejester source*/
                    token = strtok(NULL, " ,\n\t");
                    if(token)                   /* if there is more then one oprend */
                        if(token[0]!='@')      /* and the second oprend isn't also a rejester need more word*/
                            ic++;
                }
                else /* token is not a rejster*/
                {
                    ic++;
                    token = strtok(NULL, " ,\n\t");
                    if(token) /* if there is more then one oprend */
                        ic++;
                }
            }
            return ic;
        }
    return ic;
}
















































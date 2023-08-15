#include "first_pass.h"

symbol_dict *first_pass(FILE * pre_assembled_file,int *DC,int *IC,int *error_first_pass)
{
    int line_number = 1; 
    int ic,dc; /* count current number instruction/data */
    char line[MAX_LINE];
    char label_name[MAX_SYMBOL];
    int internal,externl;
    symbol_node *current_nude=NULL;
    symbol_dict *symbol_table=validate_calloc_symbol(1,sizeof(symbol_dict));

    

    while(fgets(line, MAX_LINE, pre_assembled_file))
    {
        if(is_valid_length(line,line_number,pre_assembled_file))
        {
            if (validate_line(line, line_number))
            {
                ic=count_instructions(line);
                dc=count_data(line);        
                if(is_label_line(line))         
                {
		    externl = is_extern(line);                            
		    internal = is_entry(line);                           
                    get_label_name(line,label_name,externl); /* label name of external will be ".extern" so allways go to insert_extern */
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
    rewind(pre_assembled_file);
    return symbol_table;
}





/* HILLEL FONCTIONS ALL FONCTION ASSUME LINE IS CORRECT */
int is_extern(char * line)
{

    char copy_line[MAX_LINE], *token;
    strcpy(copy_line, line);
    token = strtok(copy_line, " \n\t");
    if(!strcmp(token, ".extern"))
        return true;
    token = strtok(copy_line, " \n\t");
    if(!strcmp(token, ".extern"))
        return true;
    return false;
}

int is_entry(char * line)
{

    char copy_line[MAX_LINE], *token;
    strcpy(copy_line, line);
    token = strtok(copy_line, " \n\t");
    if(!strcmp(token, ".entry"))
        return true;
    token = strtok(copy_line, " \n\t");
    if(!strcmp(token, ".entry"))
        return true;
    return false;
}

void get_label_name(char * line,char * name,int externl)
{
    char copy_line[82], *token;
    if(externl)
    {
        strcpy(name, ".extern");
        return;
    }
    strcpy(copy_line, line);
    token = strtok(copy_line, " \n\t");
    if(!strcmp(token, ".entry"))
    {
        token = strtok(NULL, " \n\t");  /* moving to name from ".entry" */
        strcpy(name, token);
    }
    else
    {
        token[strlen(token) -1] = '\0' ; /* deleting ":" */
        strcpy(name, token);
        token = strtok(NULL, " \n\t");
        if(!strcmp(token, ".entry"))
        {
            token = strtok(NULL, " \n\t");
            strcpy(name, token);
        }
    }
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
    return true;
}


void insert_extern(symbol_dict *symbol_table,char *line,int line_number,int *error_first_pass)
{
    char copy_line[MAX_LINE], *label_name;
    symbol_node *current_nude;

    strcpy(copy_line, line);
    label_name = strtok(copy_line, " ,\n\t");  /* now label_name = .extern OR label_name=label: AND the .extern is after unsued label */
    if(strcmp(label_name, ".extern"))         /* line has the form: unsued_label: .extern first_label ... */
        label_name = strtok(NULL, " ,\n\t");
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
    if(!token)
        return dc;    
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
    else if(!strcmp(token, ".string"))
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
    if(!token)
        return ic;
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
















































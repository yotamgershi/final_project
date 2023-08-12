#include "first_pass.h"

symbol_dict *first_pass(FILE * pre_assembled_file,int *DC,int *IC,int *error_first_pass)
{
    int line_number = 1; 
    int ic,dc; /* count current number instruction/data */
    char line[MAX_LINE];
    char label_name[MAX_SYMBOL];
    int internal,externl;
    symbol_node *current_nude=NULL;
    symbol_dict *symbol_table=new_symbol_dict();

    

    while(fgets(line, MAX_LINE, pre_assembled_file))/*reading file...*/
    {
        if(is_valid_length(line,line_number,pre_assembled_file)) /* if line has valid length */
        {
            if (1) /* if line has valid syntax */
            {
                if(is_empty_or_comment_line(line))
                    continue;
                ic=count_instructions(line); /* count the number of instruction word in the line to encode */
                dc=count_data(line);        /* count the number of data word in the line to encode */
                if(is_label_line(line))    /* cheak if there is a label definition or declartion in line */      
                {
		    externl = is_extern(line);                            
		    internal = is_entry(line);                           
                    get_label_name(line,label_name,externl); /* label name of external will be ".extern" so allways go to insert_extern */
		    if((current_nude=find(symbol_table, label_name)) == NULL) /* if there's no defintion/declartion of symbol with the same name */
                    {
                        if(!internal && !externl && dc)
                            insert(symbol_table, label_name, *DC+KS, line_number,externl, internal,true); /* addrres=*DC+100,is_data=true */
                        else if(!internal && !externl && !dc)
                            insert(symbol_table, label_name,*IC+KS, line_number,externl, internal,false);/* addrres=*IC+100,is_data=false */    
		        else if(internal)
                            insert(symbol_table, label_name, -1, line_number,externl, internal,false);
                        else /* externl */
                            insert_extern(symbol_table,line,line_number,error_first_pass); /*could be number of label in line */
                    }
		    else /*there is a defintion/declartion of symbol with the same name so need to cheak if this an error or legit */
                        is_repeat_def(current_nude,line_number,externl,internal,error_first_pass,*DC,*IC,dc);	
                }
                *IC+=ic;  /* updating the total number of instruction word to encode in file*/
	        *DC+=dc; /* updating the total number of data word to encode in file*/
            }
            else
                *error_first_pass=true;
        }
        else
            *error_first_pass=true;
        line_number++;       
    }
    fclose(pre_assembled_file);
    return symbol_table;
}



/*receive line, cheak if it's an empty or comment line if so return true(1), otherwise return false(0)
  we allow white spaces before ; in comment line */

int is_empty_or_comment_line(char *line)
{
    char copy_line[MAX_LINE], *token;
    strcpy(copy_line, line);
    token = strtok(copy_line, " \n\t");    
    if(!token)
        return true;
    if(token[0]==';')
        return true;
    return false;
}



/*receive line, its number, and file pointer. cheak if the length is valid if so return true(1), otherwise return false(0)
and, countinu to read file until enter to get to next line. of course print error message in case of error. */

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

/*receive line cheak if first or second word is ".extern" if either return true(1) else false(1)
  because if line start wite extern or there's (unused!!) label in the begining, and then extern, its an extern line */
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

/*receive line cheak if first or second word is ".entry" if either return true(1) else false(1)
  because if line start wite entry or there's (unused!!) label in the begining, and then entry, its an entry line */
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
/* receive line that has lable, pointer to lable_name, and a "flag" if the line is extern.
   update the lable_name to contain the name of the symbol declerd/defined in the line, unless
   its extern line, in which case it's update the name to be ".extern" because extern line could decler on number
   of lable unlike an entry declartion or definition of lable*/
void get_label_name(char * line,char * label_name,int externl)
{
    char copy_line[82], *token;
    if(externl) /*if extern update label_name to be ".extern" and end the fonction*/
    {
        strcpy(label_name, ".extern");
        return;
    }
    strcpy(copy_line, line);
    token = strtok(copy_line, " \n\t"); /* token=first word in line */
    if(!strcmp(token, ".entry")) /*if the line is: .entry label */
    {
        token = strtok(NULL, " \n\t");  /* moving to name from ".entry" */
        strcpy(label_name, token);
    }
    else/*line is: label: comand/data OR L1: .extern L2 in which case label_name=label OR L2 ,L1 is unused label that can be defined in other places in the file*/
    {
        token[strlen(token) -1] = '\0' ; /* deleting ":" in label OR L1 */
        strcpy(label_name, token);      /* now label_name=label/L1 */
        token = strtok(NULL, " \n\t"); /* now token = second word in line */
        if(token)
            if(!strcmp(label_name, ".entry")) /* meaning we now in case 2, ie: L1: .entry L2 */
            {
                token = strtok(NULL, " \n\t"); /* move to L2 */
                strcpy(label_name, token);    /* label_name=L2 */
            }
    }
}
/*receive node of symbole that had been declered/defined already in the file. cheak if the current line is not contradicting the former one.
  assuming that multipul declartion of entry/extern with the same name is ok, but gives a warning. */
void is_repeat_def(symbol_node *current_nude,int line_number,int externl,int internal,int *error_first_pass,int DC, int IC, int dc)
{
    if( (get_address(current_nude)) > 0) /* if the node addres is greater then 0 it means the symbol had been defined allredy*/
    {
        if(internal && get_is_entry(current_nude)) /* if the symbol is defined and declerd as entry and had been declerd as entry, agine, in this line*/
            printf("WAWNING in line: %d symbol: %s is already defined as entry", line_number, get_name(current_nude));
        else if(internal) /* sybole had been defined and now declerd as entry */
            set_is_entry(current_nude,true);
        else /* sybole had been defined and now defined agine or declerd as extern */
        {
            printf("ERROR in line: %d symbol: %s is already defined in line: %d\n", line_number, get_name(current_nude),get_line_number(current_nude));
            *error_first_pass=true;
        }
    }/*else meaing the symbol is not defined but declerd */
    else if(externl && get_is_extern(current_nude)) /* if this line is declartion of extern and the symbol had been declerd as extern, warring*/
	printf("WAWNING in line: %d symbol: %s is already defined as extern in line: %d\n", line_number,get_name(current_nude),get_line_number(current_nude));
    else if(internal && get_is_entry(current_nude))/* if this line is declartion of entry and the symbol had been declerd as entry, warring*/
	printf("WAWNING in line: %d symbol: %s is already defined as entry in line: %d\n", line_number,get_name(current_nude),get_line_number(current_nude));
    else if(internal && get_is_extern(current_nude))/* if this line is declartion of entry and the symbol had been declerd as extern, error*/
    {
        printf("ERROR in line: %d symbol: %s is already defined as extern in line: %d\n", line_number, get_name(current_nude),get_line_number(current_nude));
        *error_first_pass=true;
    }
    else if(externl && get_is_entry(current_nude)) /* if this line is declartion of extern and the symbol had been declerd as entry, warring*/
    {
        printf("ERROR in line: %d symbol: %s is already defined as entry in line: %d\n", line_number, get_name(current_nude),get_line_number(current_nude));
        *error_first_pass=true;
    }
    else /* this mean that: internal=external=false, so current line is definition of symbol that had been declerd befor */
    {
        if(get_is_extern(current_nude)) /* if the symbol declerd as extern its an error, because extern symbol can not be dfined in the same file*/
        {
            printf("ERROR in line: %d symbol: %s is already declared as extern in line: %d\n", line_number, get_name(current_nude),get_line_number(current_nude));
            *error_first_pass=true;
        }
        else /* current_nude->is_entry = true. not the definition of the lable that had been declered as entry*/
        {
            if(dc) /* its a data label so need to get addres of data */
                set_address(current_nude,DC+KS);
            else /* its not a data label so need to get addres of instruction */
                set_address(current_nude,IC+KS);
            set_is_data(current_nude,dc);     
        }
    
    }       
}

/* receive line return true(1) if the line decler or define a label, else return false(0)*/
int is_label_line(char *line)
{
    char copy_line[MAX_LINE], *token;
    strcpy(copy_line, line);
    token = strtok(copy_line, " \n\t");

    if(is_entry(line) || is_extern(line)) /* line has the form: ".entry " or ".entry " so declers a label*/
        return true;
    if(token[strlen(token) -1] != ':') /* line is not: "label: " nor the above form, so no lable */
        return false;
    return true;
}

/* receive dictionary of symbol, line that declers one or more extrn labels, the line number, and pointer to signel if there are errors
   it adds to the dictionary the labels that are declers in this line*/
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
            insert(symbol_table, label_name, -1, line_number,true, false,false); /* addres=-1 no meaning,extern=true,enternl=false,is_data=false*/
        else
            is_repeat_def(current_nude,line_number,true,false,error_first_pass,0,0,0); /* extern=true,enternl=false, and DC,IC,dc has no meaning so 0*/
        label_name = strtok(NULL, " ,\n\t");
    }
}

/*count the number of data words needed to be encod for this line, return this number */
int count_data(char *line)
{
    
    int dc=0;
    int i=0; 
    char copy_line[MAX_LINE], *token;
    strcpy(copy_line, line);
    token = strtok(copy_line, " ,\n\t");   
    if(token[strlen(token) -1] == ':') /* If token=lable: , moveing to next word */
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
         dc++; /* for the '/0' char */
    }

    return dc;
}

/*count the number of instructions words needed to be encod for this line, return this number */
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
                        if(token[0]!='@')      /* and the second oprend isn't also a rejester need one more word*/
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

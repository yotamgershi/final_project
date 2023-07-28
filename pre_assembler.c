#include "pre_assembler.h"



FILE *pre_assembler(char *file_name) {


    /*TODO: make a different function for the creation of .am files*/
    int eror_in_macro=false;
    char *input_file_name = add_suffix(file_name, ".as");
    char *dest_file_name = add_suffix(file_name, ".am");
    char line[MAX_LINE], copy_line[MAX_LINE], *token, *macro_body;
    char macro_name[MAX_MACRO]; /*Uses both as a name for the macro, and as flag whether we in the middle of the macro definition*/
    FILE *input_file, *dest_file;
    fpos_t start_pos;
    unsigned int line_number;
    size_t macro_length;
    macro_dict *macros = validate_calloc(1,sizeof(macro_dict));
    macro_node *cur = NULL;
   

    *line = *copy_line = *macro_name = macro_length = line_number = 0;
    token = macro_body = NULL;
    
    if (! ((input_file = validate_fopen(input_file_name, "r")) &&
        (dest_file = validate_fopen(dest_file_name, "w+"))) )
        return NULL; 

    while (fgets(line, MAX_LINE, input_file))
    {
        if(strlen(line) ==  MAX_LINE -1 && !(*macro_name))/*if line is too long we copy it entierly to am file, and F-P will handle*/
	{
		if(line[MAX_LINE -1] != '\n')/*maybe not too long after all..need to cheak */
		{	
			while(strlen(line) ==  MAX_LINE -1)
			{
			    fprintf(dest_file, "%s", line);
			    if(line[MAX_LINE -1] != '\n') /*not the end of the line, cuntinue to read the line */
		                 fgets(line, MAX_LINE, input_file);
			    else
				 break;
			}
			if(strlen(line) < MAX_LINE -1)
			     fprintf(dest_file, "%s", line);
			continue;
		}
		

	}
        line_number++;
        strcpy(copy_line, line);
        token = strtok(line, DELIMITER);
        if (!token)
            continue;
        if (!strcmp(token, "mcro") && !(*macro_name))
        {
            token = strtok(NULL, DELIMITER);
            if (! valid_macro_def(token))
            {
		if(!eror_in_macro)
                    printf("Errors in file: %s, so we will not create an \"am\" file.\n\n",input_file_name);
                eror_in_macro=true;
                printf("ERROR: macro definition not valid in line %d.\n", line_number);
		printf("The line was: %s", copy_line);
		printf("We assume that valid def of macro is: start with a letter, max 31 char, and not a saved word.\n \n");
                continue;
            }
            strcpy(macro_name, token);

        }
        else if (*macro_name && strcmp(token, "endmcro"))
        {

            if (!macro_body)
            {
                fgetpos(input_file, &start_pos);
                macro_length = calculate_macro_length(input_file, start_pos, line);
                macro_body = (char*)validate_calloc(macro_length, sizeof(char));
                strcpy(macro_body, copy_line);
                fsetpos(input_file, &start_pos);

            }
            else
                strcat(macro_body, copy_line);
        }
        else if (!strcmp(token, "endmcro") && *macro_name )
        {
	    token = strtok(NULL, DELIMITER);
	    if( token != NULL )/* something after endmacro, so its not the endmacro */
	    {
	         strcat(macro_body, copy_line);  /* Therefor we put the line in the macro's body */
		 continue;
	    }
            insert_macro_dict(macros, macro_name, macro_body);
            macro_length = 0;
            *macro_name = 0;
            macro_body = NULL;
        }
        else if ((cur = find_macro_dict(macros, token))) {
            fprintf(dest_file,"%s", cur->body);
        }
        else
            fprintf(dest_file, "%s", copy_line);
    }
    rewind(dest_file);
    fclose(input_file);
    free_dictionary(macros);
    if(!eror_in_macro && !(*macro_name))
        return dest_file;
    if(*macro_name)
    {
           printf("\nErrors in file: %s. There is definition of mcro but not \"end mcro\" command.\n",input_file_name);
	   printf("So we will not create an \"am\" file.\n\n");
    }
    if(remove(dest_file_name))
        printf("\nThere is an eror in macro, but could not destroy the \"am\" file.\n");
    return NULL;
}

FILE *validate_fopen(const char *file_name, char *mode) {
    FILE *file = fopen(file_name, mode);
    if (file == NULL) {
        printf("File not opened.\n");
        return NULL;
    }
    return file;
}

char *add_suffix(char *file_name, char *suffix) {
    size_t new_size = strlen(file_name) + strlen(suffix) + 5;
    char *new_name = (char*)validate_calloc(new_size,sizeof(char));
    strcpy(new_name, file_name);
    strcat(new_name, suffix);
    return new_name;
}

size_t calculate_macro_length(FILE *input_file, fpos_t start_pos,char line[])
{
    
    char *token;
    size_t length = 0;



    do
    {
        token = strtok(line, DELIMITER);
        if (!token)
            continue;
        if (!strcmp(token, "endmcro"))
            break;
        length += MAX_LINE;
    }while (fgets(line, MAX_LINE, input_file));

    return length;
}

int valid_macro_def(char * token) /* recive pointer to tokens of macro def, and return 1 if valid or 0 else. */
{

	if(!token) /* no macro name */
		return false;
	if(!isalpha(*token) || is_instruction(token) || strlen(token)>MAX_MACRO) /* doesn't start with a letter or its instruction or macro name too long */
		return false;
	token = strtok(NULL, DELIMITER);
	if( token != NULL ) /* something after macro name*/
		return false;	

	return true;
}
int is_instruction(char * str)
{
	int i;
	char  instruction_table[16][5] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};
	for(i=0; i<16; i++)
	   if(!strcmp(str, instruction_table[i]))/* str  is opretor*/
		return true;
	return false;
}

 


#include "pre_assembler.h"

FILE *pre_assembler(char *file_name) {


    /*TODO: make a different function for the creation of .am files*/
    int eror_in_mcro=false;
    char input_file_name[MAX_LINE];
    char dest_file_name[MAX_LINE];
    char line[MAX_LINE], copy_line[MAX_LINE], *token, *mcro_body;
    char mcro_name[MAX_SYMBOL]; /*Uses both as a name for the mcro, and as flag whether we in the middle of the mcro definition*/
    FILE *input_file, *dest_file;
    fpos_t start_pos;
    unsigned int line_number;
    size_t mcro_length;
    mcro_dict *mcros =new_dict();
    mcro_node *cur = NULL;
    
    strcpy(input_file_name,file_name); 
    strcat(input_file_name,".as");
    strcpy(dest_file_name,file_name); 
    strcat(dest_file_name,".am");

    *line = *copy_line = *mcro_name = mcro_length = line_number = 0;
    token = mcro_body = NULL;
    
    if (! ((input_file = validate_fopen(input_file_name, "r")) &&
        (dest_file = validate_fopen(dest_file_name, "w+"))) )
        return NULL; 

    while (fgets(line, MAX_LINE, input_file))
    {
        
        
        line_number++;
        if((strlen(line) ==  MAX_LINE -1) && (line[MAX_LINE -1] != '\n') && !(*mcro_name)) /*if line is too long copy it to am file, and F-P will handle*/
        {

	    print_long_line(input_file,dest_file,line);
            continue;
        }

        strcpy(copy_line, line);
        token = strtok(line, DELIMITER);

        if (!token)  /*dont copy empty line*/
            continue;
        if (!strcmp(token, "mcro") && !(*mcro_name)) /*starting definition of mcro*/
        {
            if (! valid_mcro_def(copy_line))
            {
		if(!eror_in_mcro)
                    printf("Errors in file: %s, so we will not create an \"am\" file.\n\n",input_file_name);
                eror_in_mcro=true;
                printf("ERROR: mcro definition not valid in line %d.\n", line_number);
		printf("The line was: %s", copy_line);
		printf("We assume that valid def of mcro is: start with a letter, max 31 char, and not a saved word.\n \n");
            }
            else
               {
                   token = strtok(copy_line, DELIMITER);
                   token = strtok(NULL, DELIMITER);
                   strcpy(mcro_name, token);
               }
        }
        else if (*mcro_name && strcmp(token, "endmcro"))
        {
            
            if (!mcro_body)
            {
                fgetpos(input_file, &start_pos);
                mcro_length = calculate_mcro_length(input_file, start_pos, line);
                mcro_body = (char*)validate_calloc(mcro_length, sizeof(char));
                strcpy(mcro_body, copy_line);
                fsetpos(input_file, &start_pos);

            }
            else
                strcat(mcro_body, copy_line);
            if((strlen(line) ==  MAX_LINE -1) && (line[MAX_LINE -1] != '\n'))
            {
               fgets(line, MAX_LINE, input_file);
               while(strlen(line) ==  MAX_LINE -1)
               {
                   strcpy(mcro_body, copy_line);
                   if(line[MAX_LINE -1] != '\n') /*not the end of the line, cuntinue to read the line */ 
                       fgets(line, MAX_LINE, input_file);
                   else
	               break;

               }
               if(strlen(line) < MAX_LINE -1)
                   strcpy(mcro_body, copy_line);
            }
        }
        else if (!strcmp(token, "endmcro") && *mcro_name )
        {
            
	    token = strtok(NULL, DELIMITER);
	    if( token != NULL )/* something after endmcro, so its not "the" endmcro */
	    {
	         strcat(mcro_body, copy_line);  /* Therefor we put the line in the mcro's body */
		 continue;
	    }
            insert_mcro_dict(mcros, mcro_name, mcro_body);
            mcro_length = 0;
            *mcro_name = 0;
            mcro_body = NULL;
        }
        else if ((cur = find_mcro_dict(mcros, token)))
        {
            token = strtok(NULL, DELIMITER);
            if(token == NULL)
                fprintf(dest_file,"%s", get_mcro_body(cur));
            else /* something after mcro name, so its not corect use of mcro */
                fprintf(dest_file, "%s", copy_line);
        }
        else
            fprintf(dest_file, "%s", copy_line);
    }
    rewind(dest_file);
    fclose(input_file);
    free_dictionary(mcros);
    if(!eror_in_mcro && !(*mcro_name))
        return dest_file;
    if(*mcro_name)
    {
           printf("\nErrors in file: %s. There is definition of mcro but not \"end mcro\" command.\n",input_file_name);
	   printf("So we will not create an \"am\" file.\n\n");
    }
    if(remove(dest_file_name))
        printf("\nThere is an eror in mcro, but could not destroy the \"am\" file.\n");
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
/*
char *add_suffix(char *file_name, char *suffix) {
    size_t new_size = strlen(file_name) + strlen(suffix) + 5;
    char *new_name = (char*)validate_calloc(new_size,sizeof(char));
    strcpy(new_name, file_name);
    strcat(new_name, suffix);
    return new_name;
} */
void print_long_line(FILE * input_file, FILE * dest_file,char *line)
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
}


size_t calculate_mcro_length(FILE *input_file, fpos_t start_pos,char line[])
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

int valid_mcro_def(char * line) /* recive pointer to a line of mcro def, and return 1 if valid or 0 else. */
{
	char *token,copy_line[MAX_LINE];
        strcpy(copy_line, line);
        token = strtok(copy_line, DELIMITER); /* token="mcro" */
        token = strtok(NULL, DELIMITER); /* token= maco name */

	if(!token) /* no mcro name */
		return false;
	if(!isalpha(*token) || is_instruction(token) || strlen(token)>MAX_SYMBOL) /* doesn't start with a letter or its instruction or mcro name too long */
		return false;
        token = strtok(NULL, DELIMITER);
	if( token != NULL ) /* something after mcro name*/
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


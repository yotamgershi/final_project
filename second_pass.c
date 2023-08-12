#include "second_pass.h"


void second_pass(char *file_name,int DC,int IC,symbol_dict *symbol_table)
{
    
    char data_image[MAX_FILE_SIZE][BiW];
    char instruc_image[MAX_FILE_SIZE][BiW];
    int line_number = 1; 
    int dc=0,ic=0;
    int error=1;   /* flag with the meaning: 0=no_error,1=(no ext_file && no error) , 2=error */
    char line[MAX_LINE];
    char am_file_name[MAX_LINE];
    char ext_file_name[MAX_LINE];
    char ent_file_name[MAX_LINE];
    char ob_file_name[MAX_LINE];
    FILE *am_file=NULL, *ext_file=NULL, *ent_file=NULL, *ob_file=NULL;

    sets_files_name(file_name,am_file_name,ext_file_name,ent_file_name,ob_file_name);

    if( (am_file=fopen(am_file_name,"r")) == NULL || (ext_file=fopen(ext_file_name,"w")) == NULL
       || (ent_file=fopen(ent_file_name,"w")) == NULL || (ob_file=fopen(ob_file_name,"w")) == NULL )
        return;

    update_data_address(symbol_table,IC); /* updating  addreses of data symboles, to be after all instruction encoding */

    while(fgets(line, MAX_LINE, am_file))  /* read am file and encode all lines to data/instruction image*/
    {
        if(is_empty_or_comment_line(line))
        {
            line_number++;
            continue;
        }
        encode_data_line(data_image,line,&dc); /* if data/string line it encod it to binary and save in data_image*/  
        encode_instruction_line(instruc_image,line,&ic,line_number,ext_file,&error,symbol_table);/*if insruct line, encod it to binary and save in instruc_image*/  
        line_number++;
    }
    
    
    print_entry_to_file(ent_file,ent_file_name,symbol_table,&error);/*print entry file, if no entry destroy ent file*/
    print_tables_to_file(instruc_image,data_image,DC,IC,ob_file);/*print instruc_image and data_image to ob file in BASE-64 */

    if(error==2) /*thers some error destroy files */
    {
        if(remove(ext_file_name) || remove(ob_file_name))
            printf("\n\n There is an eror, but could not destroy the files in second pass\n");
    }
    else if(error==1) /*no extern, destroy ext file*/
    {
        fclose(ob_file);
        if(remove(ext_file_name))
            printf("\n\n There is an eror, but could not destroy the files in second pass\n");
    }
    else
    {
        fclose(ext_file);
        fclose(ob_file);   
    }
    fclose(am_file);
}

/* receive file name, and pointers to diffrent extantion file name, and sets the to that name with needed extantion */
void sets_files_name(char *file_name,char *am_file_name,char *ext_file_name,char *ent_file_name,char *ob_file_name)
{
    strcpy(am_file_name,file_name); 
    strcat(am_file_name,".am");
    strcpy(ext_file_name,file_name); 
    strcat(ext_file_name,".ext");
    strcpy(ent_file_name,file_name); 
    strcat(ent_file_name,".ent");
    strcpy(ob_file_name,file_name); 
    strcat(ob_file_name,".ob");
}

/*receive symbol dictionary, IC, the number of instruction words, and update data address to be after them*/
void update_data_address(symbol_dict *symbol_table,int IC)
{
    int i;
    symbol_node *head;
    for(i = 0; i < NUM_OF_LETTERS; i++) 
    {
        
        head=get_page(symbol_table,i);
        while(head)
        {
            if(get_is_data(head));
                set_address(head,get_address(head)+IC);
            head = get_next_symbol(head);
        }
    }
}
/*receive data image and line if this a data line encode it to binary and update dc to point at next free word in the data image */
void encode_data_line(char data_image[][BiW] ,char *line,int *dc)
{
    char copy_line[MAX_LINE], *token;

    strcpy(copy_line, line);
    token = strtok(copy_line, " ,\n\t");

    if(token[strlen(token) -1] == ':') /* If token=lable: */
        token = strtok(NULL, " ,\n\t");
    if(!strcmp(token, ".data")) /*if this data line*/
    {
        token = strtok(NULL, " ,\n\t"); /* token=first intger  */
        while(token)
        {
            encode_data(data_image[*dc],atoi(token)); /*encode number to binary */
            *dc+=1;
            token = strtok(NULL, " ,\n\t"); /* token=next intger  */

        }
    }
    else if(!strcmp(token, ".string"))
    {
         int i=0;
         while(line[i++] != '"'); /* now line[i] = first char in the string */
         while(line[i] != '"')
         {
             encode_data(data_image[*dc],(int)line[i]); /*encode char to binart by ascii value*/
             *dc+=1;
             i++;
         }
         encode_data(data_image[*dc],'\0'); /* ads char of end of string */
         *dc+=1;
    }

}

/*recive pointer to word, and data, encode data to binary and saved in word*/
void encode_data(char *word,int data)
{
    int i,mask=1;
    
    for(i=0;i<BiW;i++)
    {
        if(mask & data)
            word[i]='1';
        else
            word[i]='0';
        mask<<=1;
    }
}

/*receive instruction image, line, pointer to ic-instruction count, line number, ext_file, pointer to error, symbol table
  if this instruction word encode it, if thers lable in use update ext_file if its extern, and error if no sach symbol*/
void encode_instruction_line(char instruc_image[][BiW],char *line,int *ic,int line_number,FILE *ext_file,int *error,symbol_dict *symbol_table)
{
    int i=0; 
    char  instruction_table[16][5] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};
    char copy_line[MAX_LINE], *token,copy_oprend[MAX_SYMBOL];

    strcpy(copy_line, line);
    token = strtok(copy_line, " ,\n\t");

    if(token[strlen(token) -1] == ':') /* If token=label: */
        token = strtok(NULL, " ,\n\t");
    for(i=0; i<16; i++)
        if(!strcmp(token, instruction_table[i])) 
        {
            encode_instruction_opcode(instruc_image[*ic],i); /*encode opcode, now word=000----00000, ----=opcode*/
            token = strtok(NULL, " ,\n\t");
            if(token) /* if there is oprend */
            {
                strcpy(copy_oprend, token); /*save the first_oprend*/
                encode_destantion_oprend(instruc_image[*ic],token); /* treat first oprend as destantion in case thers only one */
                if(token[0]=='@') /* if first oprend is rejestr */
                {
                    encode_first_rej(instruc_image[*ic+1],(int)token[2]-'0');                          
                    token = strtok(NULL, " ,\n\t");
                    if(token)                   /* if there is second oprend */
                    {
                        encode_sourse_oprend(instruc_image[*ic],copy_oprend); /*discoverd ther's two oprends, so encode first as sourse*/
                        encode_destantion_oprend(instruc_image[*ic],token);
                        if(token[0]=='@')
                        {
                            encode_second_rej(instruc_image[*ic+1],(int)token[2]-'0',true); 
                            *ic-=1; /*both first and second oprends are rej, so only two words*/
                        }
                        else if(!isalpha(token[0]))
                            encode_data(instruc_image[*ic+2],(atoi(token)<<2));
                        else
                            encode_label(instruc_image[*ic+2],token,line_number,ext_file,error,symbol_table,*ic+2+KS);
                        *ic+=1;
                    }         
                }
                else /* token is not a rejster*/
                {
                    if(!isalpha(token[0]))
                        encode_data(instruc_image[*ic+1],(atoi(token)<<2));
                    else
                        encode_label(instruc_image[*ic+1],token,line_number,ext_file,error,symbol_table,*ic+1+KS);
                    token = strtok(NULL, " ,\n\t");
                    if(token) /* if there is more then one oprend */
                    {
                        encode_sourse_oprend(instruc_image[*ic],copy_oprend);
                        encode_destantion_oprend(instruc_image[*ic],token);
                        if(token[0]=='@')
                            encode_second_rej(instruc_image[*ic+2],(int)token[2]-'0',false);
                        else if(!isalpha(token[0]))
                            encode_data(instruc_image[*ic+2],(atoi(token)<<2));
                        else
                            encode_label(instruc_image[*ic+2],token,line_number,ext_file,error,symbol_table,*ic+2+KS);
                        *ic+=1;
                    }     
                }
                *ic+=1;
            }
            *ic+=1;
            return;
        }
}
void encode_instruction_opcode(char *word,int command_num)
{
    
    encode_data(word,command_num<<5);
}

void encode_sourse_oprend(char *word,char *first_oprend)
{

    if(first_oprend[0]=='@')
    {
        word[11]='1';
        word[10]='0';
        word[9]='1';
    }
    else if(isalpha(*first_oprend))
    {
        word[11]='0';
        word[10]='1';
        word[9]='1';
    }
    else
    {
        word[11]='0';
        word[10]='0';
        word[9] ='1';
    }
}

void encode_destantion_oprend(char *word,char *second_oprend)
{
    if(second_oprend[0]=='@')
    {
        word[4]='1';
        word[3]='0';
        word[2]='1';
    }
    else if(isalpha(*second_oprend))
    {
        word[4]='0';
        word[3]='1';
        word[2]='1';
    }
    else
    {
        word[4]='0';
        word[3]='0';
        word[2]='1';
    }  
}

void encode_first_rej(char *word,int rejster_num)
{
    
    encode_data(word,rejster_num<<7);
}


void encode_second_rej(char *word,int rejster_num, int is_first_also_rej)
{
    int mask=1, i;
    int rej_num;

    rej_num = rejster_num<<2;
    mask<<=2;
    
    if(!is_first_also_rej)
        encode_data(word,rej_num);
    else
    {

        for(i=2;i<7;i++)
        {
            if(mask & rej_num)
                word[i]='1';
            else
                word[i]='0';
            mask<<=1;
        }
    }   

}

void encode_label(char *word,char *label_name,int line_number,FILE *ext_file,int *error,symbol_dict *symbol_table,int index)
{
    symbol_node *current_nude;
    current_nude=find(symbol_table, label_name);

    if(current_nude==NULL)
    {
        *error=2;
        printf("ERROR in line %d, symbol=%s doesn't defined",line_number,label_name);
        return;
    }

    encode_data(word,(get_address(current_nude))<<2);

    if(get_is_extern(current_nude))
    {
        encode_data(word,0);
        if(*error !=2)
            *error=0;
        word[0]='1'; /* extern */
        fprintf(ext_file, "%s	%d\n",label_name,index);
    }
    else
        word[1]='1';    
}




void print_entry_to_file(FILE *ent_file,char *ent_file_name,symbol_dict *symbol_table,int *error)
{
    
    int there_is_entry=false;
    int i;
    symbol_node *head;
    for(i = 0; i < NUM_OF_LETTERS; i++) 
    {
        head=get_page(symbol_table,i);
        while(head)
        {
            if(get_is_entry(head) && get_address(head)>0)
            {
                there_is_entry=true;
                fprintf(ent_file, "%s    %d\n",get_name(head),get_address(head));
            }
            else if(get_is_entry(head))
            {
               printf("ERROR symbol: %s declerd as entry in line %d but has no definition in file\n",get_name(head),get_line_number(head));
               *error=2;
            }
            head = get_next_symbol(head);
        }
    }
    if(*error==2 || !there_is_entry)
    {
        if(remove(ent_file_name))
            printf("\n\n There is an eror, but could not destroy the files in second pass\n");
    }
    else
        fclose(ent_file);
    return;
}




void print_tables_to_file(char instruc_image[][BiW],char data_image[][BiW],int DC,int IC,FILE *ob_file)
{
    int i;
    fprintf(ob_file, "%d %d\n",IC,DC);
    for(i=0; i<IC; i++)
        print_binary_word_64(instruc_image[i],ob_file);
    for(i=0; i<DC; i++)
        print_binary_word_64(data_image[i],ob_file);
}



void print_binary_word_64(char *word,FILE *ob_file)
{
    char base_64[64]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                      'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
                      '0','1','2','3','4','5','6','7','8','9','+','/'};

    int num=0;
    int i;
    int factor=1;
    int a,b;
    char c1,c2;

    for(i=0;i<BiW;i++) /* to get num equal the desimal represntaion of word */
    {
        num+= ((int)word[i] - '0')*factor;
        factor=factor*2; 
    }
 
    a=num/64;
    b=num%64;

    c1=base_64[a];
    c2=base_64[b];

    fprintf(ob_file, "%c%c\n",c1,c2);
}




















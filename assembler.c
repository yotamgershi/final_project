#include "assembler.h"



int main (int argc, char *argv[]) {

    int i;
    int DC,IC;
    int error_first_pass;
    symbol_dict *symbol_table;
    FILE *pre_assembled_file;

    if (argc == 1)
        printf("Usage: ./assembler file1 file2 ...");

    for (i = 1; i < argc; i++)
    {
        DC=0;
        IC=0;
        error_first_pass=false;
 
        pre_assembled_file = pre_assembler(argv[i]);
        if (pre_assembled_file) /* No error in pre_assembled move to first pass */
        {
            symbol_table = first_pass(pre_assembled_file,&DC,&IC,&error_first_pass); 
            if (!error_first_pass) /* No error in first pass move to second */ 
                second_pass(argv[i],DC,IC,symbol_table);
            free_dictionary_symbol(symbol_table);     
        }
    } 
    return 0;
}


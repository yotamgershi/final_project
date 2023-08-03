#include "assembler.h"

int main (int argc, char *argv[]) {
    int i;
    if (argc == 1)
        printf("Usage: ./assembler file1 file2 ...");
    for (i = 1; i < argc; i += 1)
    {
        int DC=0,IC=0;
        int error_first_pass=false;
        symbol_dict *symbol_table; 
        FILE *pre_assembled_file = pre_assembler(argv[i]);
        if (pre_assembled_file)
        {
                symbol_table = first_pass(pre_assembled_file,&DC,&IC,&error_first_pass); ] 
                if (!error_first_pass) /* No error in first pass move to second */
                {
                 /* FILE *assembled_file = second_pass(first_pass_file); /* change FILE *assembled_file to dict *DATA_IMAGE and dict *CODE_IMAGE 
                    if (assembled_file)
                    {
                        output(assembled_file);
                    }*/
                }
               /* NEED TO FREE symbol_table !!!! */
        }
    }
    
    return 0;
}


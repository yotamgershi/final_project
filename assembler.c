#include "assembler.h"

int main (int argc, char *argv[]) {
    int i;
    if (argc == 1)
        printf("Usage: ./assembler file1 file2 ...");
    for (i = 1; i < argc; i += 1)
    {
        FILE *pre_assembled_file = pre_assembler(argv[i]);
        if (pre_assembled_file)
        {
            /*FILE *first_pass_file = first_pass(pre_assembled_file); /* TODO: change FILE *first_pass_file to dict *symbol_table 
                if (first_pass_file)
                {
                    FILE *assembled_file = second_pass(first_pass_file); /* change FILE *assembled_file to dict *DATA_IMAGE and dict *CODE_IMAGE 
                    if (assembled_file)
                    {
                        output(assembled_file);
                    }
                }*/
        }
    }
    
    return 0;
}

/* hello world */


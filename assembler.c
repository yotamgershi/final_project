#include "assembler.h"



int main (int argc, char *argv[]) 
{
    printf("Test 1: %s\n", is_cmd("mov, ,") ? "Valid" : "Invalid");
    printf("Test 2: %s\n", is_cmd("add") ? "Valid" : "Invalid");
    printf("Test 3: %s\n", is_cmd("clr") ? "Valid" : "Invalid");
    printf("Test 4: %s\n", is_cmd("") ? "Valid" : "Invalid");





    /*int i;
    if (argc == 1)
        printf("Usage: ./assembler file1 file2 ...");
    for (i = 1; i < argc; i += 1)
    {
        FILE *pre_assembled_file = pre_assembler(argv[i]);

        if (pre_assembled_file)
        {
            symbol_table = first_pass(pre_assembled_file,&DC,&IC,&error_first_pass); 
            if (!error_first_pass) /* No error in first pass move to second
                second_pass(argv[i],DC,IC,symbol_table);
            free_dictionary_symbol(symbol_table);     
        }
    }*/
    
    return 0;
}


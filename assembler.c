#include "assembler.h"

int main (int argc, char *argv[]) 
{
    char line1[] = " r1, r2, r3,";
    char line2[] = " 123";
    char line3[] = ", ,";
    char line4[] = " r1, r2, ,r3, r4";
    char line5[] = " ,r1, r2";
    char line6[] = " r1, r2";
    char line7[] = "   , ";


    bool result1 = is_valid_commas(line1);
    bool result2 = is_valid_commas(line2);
    bool result3 = is_valid_commas(line3);
    bool result4 = is_valid_commas(line4);
    bool result5 = is_valid_commas(line5);


    printf("Result 1: %s\n", result1 ? "true" : "false");
    printf("Result 2: %s\n", result2 ? "true" : "false");
    printf("Result 3: %s\n", result3 ? "true" : "false");
    printf("Result 4: %s\n", result4 ? "true" : "false");
    printf("Result 5: %s\n", result5 ? "true" : "false");

    return 0;
    /*int i;
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
                }
        }
    }
    
    return 0; */
}


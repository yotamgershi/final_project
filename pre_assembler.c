#include "pre_assembler.h"

FILE *pre_assembler(char *file_name) {

    /*TODO: make a different function for the creation of .am files*/
    char *input_file_name = add_suffix(file_name, ".as");
    char dest_file_name[FILENAME_MAX] = {0};
    char line[MAX_LINE], copy_line[MAX_LINE], *token, macro_name[MAX_MACRO], *macro_def;
    FILE *input_file, *dest_file;
    unsigned int line_number = 0;
    size_t macro_length = 0;
    macro_dict *macros = validate_calloc(1,sizeof(macro_dict));
    macro_node *cur = NULL;
    printf("Starting to assembler\n");
    strcpy(dest_file_name, file_name);
    strcat(dest_file_name, ".am");

    *line = *copy_line = *macro_name = 0;
    token = macro_def = NULL;
    printf("Starting to assembler\n");
    if (! ((input_file = validate_fopen(input_file_name, "r")) &&
        (dest_file = validate_fopen(dest_file_name, "w+"))) ) {
        printf("No file\n");
        return NULL;
        }
    free(input_file_name);

    while (fgets(line, MAX_LINE, input_file))
    {
        line_number++;
        strcpy(copy_line, line);
        token = strtok(line, DELIMITER);
        if (!token)
            continue;
        if (!strcmp(token, "mcro"))
        {
            token = strtok(NULL, DELIMITER);
            if (!token)
            {
                printf("ERROR: macro name not found in line %d\n.", line_number);
                continue;
            }
            strcpy(macro_name, token);
        }
        else if (*macro_name && strcmp(token, "endmcro"))
        {
            if (!macro_def)
            {
                fpos_t start_pos;
                fgetpos(input_file, &start_pos);
                macro_length = calculate_macro_length(input_file, start_pos);
                macro_def = (char*)validate_calloc(1, macro_length);
                *macro_def = 0;
                fsetpos(input_file, &start_pos);
            }
            strcat(macro_def, copy_line);
        }
        else if (!strcmp(token, "endmcro"))
        {   
            insert_macro_dict(macros, macro_name, macro_def);
            macro_length = 0;
            *macro_name = 0;
            macro_def = NULL;
        }
        else if ((cur = find_macro_dict(macros, token))) {
            fprintf(dest_file,"%s", cur->def);
        }
        else
            fprintf(dest_file, "%s", copy_line);
    }
    rewind(dest_file);
    fclose(input_file);
    return dest_file;
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
    size_t new_size = strlen(file_name) + strlen(suffix) + 1;
    char *new_name = (char*)validate_malloc(new_size);
    strcpy(new_name, file_name);
    strcat(new_name, suffix);
    return new_name;
}

size_t calculate_macro_length(FILE *input_file, fpos_t start_pos)
{
    char line[MAX_LINE];
    char *token;
    size_t length = 0;

    fsetpos(input_file, &start_pos);

    while (fgets(line, MAX_LINE, input_file))
    {
        token = strtok(line, DELIMITER);
        if (!token)
            continue;
        if (!strcmp(token, "endmcro"))
            break;
        length += strlen(line) + 1;
    }

    return length;
}


/*  */


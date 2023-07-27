#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "symbol_dict.h"

typedef struct image
{
    /* data */
}image;

image *second_pass(symbol_dict *symbol_table); /* change FILE *assembled_file to dict *DATA_IMAGE and dict *CODE_IMAGE */

#endif
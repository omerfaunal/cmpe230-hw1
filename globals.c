#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

const char *terminals[23] = {"scalar", "vector", "matrix", "[", "]", ",", "{", "}",
                             "*", "+", "-", "tr", "(", ")", "sqrt", "choose",
                             "#", "for", "in", ":", "printsep", "print", "="};
                              // Note that numeric characters and variable names aren't included here.

struct Matrix matrices[256];  // Matrix variables will be stored here.
struct Matrix* matrixListPointer = matrices; //This is a pointer for traversing matrices list.
int matrix_count = 0;
struct Scalar scalars[256];  // Scalar variables will be stored here.
struct Scalar* scalarListPointer = scalars;//This is a pointer for traversing scalars list.
int scalar_count = 0;
int line_number = 1;
FILE *out_file;
char out_file_name[4096];

/*
 * Given a line number
 * Simply prints "Error (Line number)"
 */
void error(int line) {
    printf("Error (Line %d)\n", line);
    fclose(out_file);
    remove(out_file_name);
    exit(EXIT_FAILURE);
}
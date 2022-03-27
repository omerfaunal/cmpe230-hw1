#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

#define MAX_CHAR 256

void error(int line);
char* scalarValueDeclaration(char* out, char* variableName, float value);
char* matrixDeclaration(char* out, char* variableName, int columnCount, int rowCount);
char **rpn(char **line, char **out, short int start_index, short int end_index);
int *get_dimensions(char *name, int *dimensions);
int *typecheck(char **line, int size);

extern int line_number;
extern struct Scalar scalars[];
extern struct Matrix matrices[];
extern short int scalar_count;
extern short int matrix_count;

char* eval(char **line, short int size) {  // Note that size also contains the newline character at the end of the line.
    if(size == 1) {
        return "\n";
    }

    // Scalar Declaration
    // TODO: What to do when a previously declared value is declared again?
    if(strcmp(line[0], "scalar") == 0) {
        if(size != 3) {
            error(line_number);
            return "error";
        }
        char out[MAX_CHAR + 50];
        return scalarValueDeclaration(out, line[1], 0);
    }

    // Vector declaration
    // TODO: What to do when a previously declared value is declared again?
    if(strcmp(line[0], "vector") == 0) {
        if(size != 6 || strcmp(line[2], "[") != 0 || strcmp(line[4], "]") != 0) {
            error(line_number);
            return "error";
        }
        if(strchr(line[3], (int) '.') != NULL) {
            error(line_number);
            return "error";
        }
        char out[MAX_CHAR + 50];
        return matrixDeclaration(out, line[1], 1, atoi(line[3]));
    }

    // Matrix declaration
    // TODO: What to do when a previously declared value is declared again?
    if(strcmp(line[0], "matrix") == 0) {
        if(size != 8 || strcmp(line[2], "[") != 0 || strcmp(line[4], ",") != 0 || strcmp(line[6], "]") != 0) {
            error(line_number);
            return "error";
        }
        if(strchr(line[3], (int) '.') != NULL || strchr(line[5], (int) '.') != NULL) {
            error(line_number);
            return "error";
        }
        char out[MAX_CHAR + 50];
        return matrixDeclaration(out, line[1], atoi(line[5]), atoi(line[3]));
    }


    int dimensions[2];
    if(get_dimensions(line[0], dimensions) != NULL) {
        if(dimensions[0] == 1 && dimensions[1] == 1) {
            // Scalar assignment
            if (strcmp(line[1], "=") != 0) {
                error(line_number);
                return "error";
            }
            char *out[size - 3];
            int *rhs_dims = typecheck(rpn(line, out, 2, size - 2), size - 3);
            if(rhs_dims[0] == 1 && rhs_dims[1] == 1) {
                // TODO: When the code reaches here, typechecking should be concluded with no errors, so we should
                // return the line that will be printed to the .c file.
            } else {
                error(line_number);
            }
        }
    }

}

int *typecheck(char **line, int size) {
    // char **line is the list of tokens of the expression in Reverse Polish Notation.
    // TODO: Matrix indexing
    int (*stack)[2] = (int(*)[2]) calloc(size, sizeof(int[2]));
    int (*stack_begin)[2] = stack;
    for(int i = 0; i < size; i++) {
        if(strcmp(line[i], "*") == 0) {
            // Multiplication
            if(stack - stack_begin < 2) {
                error(line_number);
            }
            stack--; int r2 = (*stack)[0]; int c2 = (*stack)[1];
            stack--; int r1 = (*stack)[0]; int c1 = (*stack)[1];
            if(r1 == 1 && c1 == 1) {
                (*stack)[0] = r2; (*stack)[1] = c2; stack++;
            } else if(c1 == r2) {
                (*stack)[0] = r1; (*stack)[1] = c2; stack++;
            } else {
                error(line_number);
            }
        } else if(strcmp(line[i], "+") == 0 || strcmp(line[i], "-") == 0) {
            // Addition and subtraction
            if(stack - stack_begin < 2) {
                error(line_number);
            }
            stack--; int r2 = (*stack)[0]; int c2 = (*stack)[1];
            stack--; int r1 = (*stack)[0]; int c1 = (*stack)[1];
            if(r1 == r2 && c1 == c2) {
                (*stack)[0] = r1; (*stack)[1] = c1; stack++;
            } else {
                error(line_number);
            }
        } else if(strcmp(line[i], "tr") == 0) {
            // Transposition
            if(stack - stack_begin < 1) {
                error(line_number);
            }
            stack--; int r1 = (*stack)[0]; int c1 = (*stack)[1];
            (*stack)[0] = c1; (*stack)[1] = r1; stack++;
        } else if(strcmp(line[i], "sqrt") == 0) {
            // Square root
            if(stack - stack_begin < 1) {
                error(line_number);
            }
            stack--; int r1 = (*stack)[0]; int c1 = (*stack)[1];
            if(r1 == 1 && c1 == 1) {
                (*stack)[0] = r1; (*stack)[1] = c1; stack++;
            } else {
                error(line_number);
            }
        } else if(strcmp(line[i], "choose") == 0) {
            // Choose
            if(stack - stack_begin < 4) {
                error(line_number);
            }
            stack--; int r4 = (*stack)[0]; int c4 = (*stack)[1];
            stack--; int r3 = (*stack)[0]; int c3 = (*stack)[1];
            stack--; int r2 = (*stack)[0]; int c2 = (*stack)[1];
            stack--; int r1 = (*stack)[0]; int c1 = (*stack)[1];
            if(r1 == 1 && r2 == 1 && r3 == 1 && r4 == 1 && c1 == 1 && c2 == 1 && c3 == 1 && c4 == 1) {
                (*stack)[0] = 1; (*stack)[1] = 1; stack++;
            } else {
                error(line_number);
            }
        } else if(isdigit((int) line[i][0]) || line[i][0] == '.') {
            // Number
            (*stack)[0] = 1; (*stack)[1] = 1; stack++;
        } else {
            // Variable
            int dim[2];
            if(get_dimensions(line[i], dim) == NULL) {
                error(line_number);
            }
            (*stack)[0] = dim[0]; (*stack)[1] = dim[1]; stack++;
        }
    }

    if(stack - stack_begin != 1) {
        error(line_number);
    }

    int *out = (int*) calloc(2, sizeof(int));
    out[0] = (*stack_begin)[0]; out[1] = (*stack_begin)[1];
    free(stack);
    return out;

}

int *get_dimensions(char *name, int *dimensions) {
    // This returns the dimensions of a variable if it exists, or NULL otherwise.
    for(int i = 0; i < scalar_count; i++) {
        if(strcmp(name, scalars[i].name) == 0) {
            dimensions[0] = 1;
            dimensions[1] = 1;
            return dimensions;
        }
    }
    for(int i = 0; i < matrix_count; i++) {
        if(strcmp(name, matrices[i].name) == 0) {
            dimensions[0] = matrices[i].row_count;
            dimensions[1] = matrices[i].column_count;
            return dimensions;
        }
    }
    return NULL;
}

char **rpn(char **line, char **out, short int start_index, short int end_index) {
    // This function uses Dijkstra's Shunting-yard Algorithm to convert a mathematical expression to Reverse Polish
    // Notation. int start_index and int end_index specify where the expression to be converted is situated inside the
    // line.
    short int out_index = 0;
    char **operator_stack = (char**) calloc(end_index - start_index + 1, sizeof(char*));
    char **operator_stack_begin = operator_stack;
    for(int i = start_index; i <= end_index; i++) {
        char *token = line[i];
        int dimensions[2];
        if(isdigit((int) token[0]) || token[0] == '.' || get_dimensions(token, dimensions) != NULL) {
            // If the token is a number or variable
            out[out_index] = token;
            out_index++;
        } else if(strcmp(token, "tr") == 0 || strcmp(token, "sqrt") == 0 || strcmp(token, "choose") == 0) {
            // If the token is a function
            *operator_stack = token;
            operator_stack++;
        } else if(strcmp(token, "*") == 0 || strcmp(token, "+") == 0 || strcmp(token, "-") == 0) {
            // If the token is a binary operator
            while(operator_stack != operator_stack_begin && (strcmp(*(operator_stack - 1), "*") == 0 ||
            (strcmp(*(operator_stack - 1), "+") == 0 && strcmp(token, "*") != 0) ||
            (strcmp(*(operator_stack - 1), "-") == 0 && strcmp(token, "*") != 0))) {
                operator_stack--;
                out[out_index] = *operator_stack;
                out_index++;
            }
            *operator_stack = token;
            operator_stack++;
        } else if(strcmp(token, "(") == 0) {
            // If the token is a left parenthesis
            *operator_stack = token;
            operator_stack++;
        } else if(strcmp(token, ")") == 0) {
            // If the token is a right parenthesis
            while(operator_stack != operator_stack_begin && strcmp(*(operator_stack - 1), "(") != 0) {
                operator_stack--;
                out[out_index] = *operator_stack;
                out_index++;
            }
            if(operator_stack == operator_stack_begin) {
                error(line_number);
            }
            operator_stack--;
            if(operator_stack != operator_stack_begin && (strcmp(*(operator_stack - 1), "tr") == 0 ||
            strcmp(*(operator_stack - 1), "sqrt") == 0 || strcmp(*(operator_stack - 1), "choose") == 0)) {
                operator_stack--;
                out[out_index] = *operator_stack;
                out_index++;
            }
        } else {
            error(line_number);
        }
    }

    while(operator_stack != operator_stack_begin) {
        operator_stack--;
        if(strcmp(*operator_stack, "(") == 0) {
            error(line_number);
        }
        out[out_index] = *operator_stack;
        out_index++;
    }

    free(operator_stack);
    return out;
}
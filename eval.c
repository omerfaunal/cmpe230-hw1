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
int *typecheck(char **line, int size, char **ptranslated);
char* matrixAssignment(char* out, char* variableName, int arraySize);
char* singleForLoop(char* out, char* expr1, char* expr2, char* expr3);
char* doubleForLoop(char* out, char* expr1, char* expr2, char* expr3, char* expr4, char* expr5, char* expr6);
char* printId(char* out, float id);
char* printSep();

extern int line_number;
extern struct Scalar scalars[];
extern struct Matrix matrices[];
extern short int scalar_count;
extern short int matrix_count;

short int for_loop_open = 0;  // 0: There isn't a for loop currently open; 1: There is a single for loop currently open,
                              // 2: There is a double for loop currently open.

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
        int row_count = atoi(line[3]);
        return matrixDeclaration(out, line[1], 1, row_count);
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
            if (size < 4 || strcmp(line[1], "=") != 0) {
                error(line_number);
                return "error";
            }
            char *out[size - 3];
            char* translated = NULL;
            int *rhs_dims = typecheck(rpn(line, out, 2, size - 2), size - 3, &translated);
            if(rhs_dims[0] == 1 && rhs_dims[1] == 1) {
                char *final_line = (char*) calloc(512, sizeof(char));
                snprintf(final_line, 512, "%s = %s;\n", line[0], translated);
                return final_line;
            } else {
                error(line_number);
            }
        } else {
            // Matrix assignment
            if (size < 4 || strcmp(line[1], "=") != 0) {
                error(line_number);
                return "error";
            }
            if(strcmp(line[2], "{") == 0 && strcmp(line[size - 2], "}") == 0) {
                // Explicit matrix assignment, i.e. all components of a matrix are listed inside curly brackets
                if(size < 6) {
                    error(line_number);
                    return "error";
                }
                for(int i = 3; i < size - 2; i++) {
                    if(!(isdigit((int) line[i][0]) || line[i][0] == '.')) {
                        error(line_number);
                        return "error";
                    }
                }
                if(size - 5 != dimensions[0] * dimensions[1]) {
                    error(line_number);
                    return "error";
                }
                char *final_line = (char*) calloc(512, sizeof(char));
                // TODO: Looks like this function isn't working:
                return matrixAssignment(final_line, line[0], dimensions[0] * dimensions[1]);

            } else {
                // Non-explicit matrix assignment
                char *out[size - 3];
                char* translated = NULL;
                int *rhs_dims = typecheck(rpn(line, out, 2, size - 2), size - 3, &translated);
                if(rhs_dims[0] == dimensions[0] && rhs_dims[1] == dimensions[1]) {
                    char *final_line = (char*) calloc(512, sizeof(char));
                    snprintf(final_line, 512, "%s = %s", line[0], translated);
                    return final_line;
                } else {
                    error(line_number);
                }
            }
        }
    }

    if(strcmp(line[0], "for") == 0) {
        // For loop
        if(for_loop_open > 0) {
            error(line_number);
            return "error";
        }
        if(size == 12) {
            // Single for loop
            for_loop_open = 1;
            // TODO: Need to check expression
            if(strcmp(line[1], "(") != 0 || strcmp(line[3], "in") != 0 || strcmp(line[5], ":") != 0 ||
            strcmp(line[7], ":") != 0 || strcmp(line[9], ")") != 0 || strcmp(line[10], "{") != 0) {
                error(line_number);
                return "error";
            }

            int dim[2];
            for(int token_index = 4; token_index <= 8; token_index += 2) {
                if (get_dimensions(line[token_index], dim) == NULL) {
                    for (int i = 0; i < strlen(line[token_index]); i++) {
                        if (!isdigit((int) line[token_index][i])) {
                            error(line_number);
                            return "error";
                        }
                    }
                } else {
                    if (!(dim[0] == 1 && dim[1] == 1)) {
                        error(line_number);
                        return "error";
                    }
                }
            }

            char *final_line = (char*) calloc(512, sizeof(char));
            return singleForLoop(final_line, line[4], line[6], line[8]);

        } else if(size == 20) {
            // Double for loop
            for_loop_open = 2;
            if(strcmp(line[1], "(") != 0 || strcmp(line[3], ",") != 0 || strcmp(line[5], "in") != 0 ||
            strcmp(line[7], ":") != 0 || strcmp(line[9], ":") != 0 || strcmp(line[11], ",") != 0 ||
            strcmp(line[13], ":") != 0 || strcmp(line[15], ":") != 0 || strcmp(line[17], ")") != 0 ||
            strcmp(line[18], "{") != 0) {
                error(line_number);
                return "error";
            }
            int dim[2];
            for(int token_index = 6; token_index <= 16; token_index += 2) {
                if (get_dimensions(line[token_index], dim) == NULL) {
                    for (int i = 0; i < strlen(line[token_index]); i++) {
                        if (!isdigit((int) line[token_index][i])) {
                            error(line_number);
                            return "error";
                        }
                    }
                } else {
                    if (!(dim[0] == 1 && dim[1] == 1)) {
                        error(line_number);
                        return "error";
                    }
                }
            }

            char *final_line = (char*) calloc(512, sizeof(char));
            return doubleForLoop(final_line, line[6], line[8], line[10], line[12], line[14], line[16]);

        } else {
            error(line_number);
            return "error";
        }
    }

    if(strcmp(line[0], "}") == 0) {
        // End of for loop
        if(size != 2) {
            error(line_number);
            return "error";
        }
        if(for_loop_open == 1) {
            for_loop_open = 0;
            return "}";
        } else if(for_loop_open == 2) {
            for_loop_open = 0;
            return "}}";
        } else{
            error(line_number);
            return "error";
        }
    }

    if(strcmp(line[0], "print") == 0) {
        // Print statement
        // TODO: Argument might be an indexed matrix
        if(size != 5 || strcmp(line[1], "(") != 0 || strcmp(line[3], ")") != 0) {
            error(line_number);
            return "error";
        }

        return printId(line[2], 0);  // TODO: What is 'float id' in this function? I don't understand
    }

    if(strcmp(line[0], "printsep") == 0) {
        // Printsep statement
        if(size != 4 || strcmp(line[1], "(") != 0 || strcmp(line[2], ")") != 0) {
            error(line_number);
            return "error";
        }
        return printSep();
    }

    error(line_number);
    return "error";

}

int *typecheck(char **line, int size, char **ptranslated) {
    // This function translates a given expression from Matlang to C. During translation, it also does typechecking.
    // If a type error is encountered, the error() function is called and translation stops. Otherwise, the translated
    // expression is stored in char *translated.
    // char **line is the list of tokens of the expression in Reverse Polish Notation.
    // TODO: Matrix indexing
    int (*stack)[2] = (int(*)[2]) calloc(size, sizeof(int[2]));
    int (*stack_begin)[2] = stack;
    char **expr_stack = (char**) calloc(size, sizeof(char*));
    char **expr_stack_begin = expr_stack;
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
            expr_stack--; char* expr2 = *expr_stack;
            expr_stack--; char* expr1 = *expr_stack;
            snprintf(*expr_stack, 512, "multiply(%s, %s)", expr1, expr2);
            expr_stack++;

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
            expr_stack--; char* expr2 = *expr_stack;
            expr_stack--; char* expr1 = *expr_stack;
            if(strcmp(line[i], "+") == 0) {
                // Addition
                // TODO: Give dimensions to add function
                snprintf(*expr_stack, 512, "add(%s, %s)", expr1, expr2);
            } else {
                // Subtraction
                snprintf(*expr_stack, 512, "subtract(%s, %s)", expr1, expr2);
            }
            expr_stack++;

        } else if(strcmp(line[i], "tr") == 0) {
            // Transposition
            if(stack - stack_begin < 1) {
                error(line_number);
            }
            stack--; int r1 = (*stack)[0]; int c1 = (*stack)[1];
            (*stack)[0] = c1; (*stack)[1] = r1; stack++;
            expr_stack--; char* expr = *expr_stack;
            // TODO: give dimensions to transpose function
            snprintf(*expr_stack, 512, "transpose(%s)", expr);
            expr_stack++;

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
            expr_stack--; char* expr = *expr_stack;
            snprintf(*expr_stack, 512, "sqrt(%s)", expr);
            expr_stack++;

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
            expr_stack--; char* expr4 = *expr_stack;
            expr_stack--; char* expr3 = *expr_stack;
            expr_stack--; char* expr2 = *expr_stack;
            expr_stack--; char* expr1 = *expr_stack;
            snprintf(*expr_stack, 512, "choose(%s, %s, %s, %s)", expr1, expr2, expr3, expr4);
            expr_stack++;

        } else if(isdigit((int) line[i][0]) || line[i][0] == '.') {
            // Number
            (*stack)[0] = 1; (*stack)[1] = 1; stack++;
            *expr_stack = line[i]; expr_stack++;
        } else {
            // Variable
            int dim[2];
            if(get_dimensions(line[i], dim) == NULL) {
                error(line_number);
            }

            if(expr_stack > expr_stack_begin && strcmp(*(expr_stack - 1), "]") == 0) {
                // TODO: Matrix indexing
            }
            else {
                (*stack)[0] = dim[0]; (*stack)[1] = dim[1]; stack++;
                *expr_stack = line[i]; expr_stack++;
            }
        }
    }

    if(stack - stack_begin != 1) {
        error(line_number);
    }

    int *out = (int*) calloc(2, sizeof(int));
    out[0] = (*stack_begin)[0]; out[1] = (*stack_begin)[1];
    free(stack_begin);
    *ptranslated = strdup(*expr_stack_begin);
    free(expr_stack_begin);
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
        if(isdigit((int) token[0]) || token[0] == '.' ||
        (get_dimensions(token, dimensions) != NULL && (i == end_index || strcmp(line[i + 1], "[") != 0))) {
            // If the token is a number or variable
            out[out_index] = token;
            out_index++;
        } else if(strcmp(token, "tr") == 0 || strcmp(token, "sqrt") == 0 || strcmp(token, "choose") == 0 ||
                (get_dimensions(token, dimensions) != NULL && i < end_index && strcmp(line[i + 1], "[") == 0)) {
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
        } else if(strcmp(token, "[") == 0) {
            // If the token is a left bracket
            *operator_stack = token;
            operator_stack++;
            out[out_index] = token;
            out_index++;
        } else if(strcmp(token, "]") == 0) {
            // If the token is a right bracket
            while(operator_stack != operator_stack_begin && strcmp(*(operator_stack - 1), "[") != 0) {
                operator_stack--;
                out[out_index] = *operator_stack;
                out_index++;
            }
            if(operator_stack == operator_stack_begin) {
                error(line_number);
            }
            out[out_index] = token;
            out_index++;
            operator_stack--;
            if(operator_stack != operator_stack_begin && get_dimensions(*(operator_stack - 1), dimensions) != NULL) {
                operator_stack--;
                out[out_index] = *operator_stack;
                out_index++;
            }
        } else if(strcmp(token, ",") == 0) {
            continue;
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
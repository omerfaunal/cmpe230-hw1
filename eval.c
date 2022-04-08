#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void error(int line);
char* declaration(char* out, char* variableName, int columnCount, int rowCount);
char **rpn(char **line, char **out, int start_index, int end_index);
int *get_dimensions(char *name, int *dimensions);
int *typecheck(char **line, int size, char **ptranslated);
char* matrixAssignment(char* out, char* variableName, char* values, int rowCount, int columnCount);
char* singleForLoop(char* out,char* id, char* expr1, char* expr2, char* expr3);
char* doubleForLoop(char* out,char* id1, char* id2, char* expr1, char* expr2, char* expr3, char* expr4, char* expr5, char* expr6);
char* callPrintId(char* out, char* variableName, int row_count, int column_count);
char* printSep();
char* callSqrt(char* out, char* variableName);
char* callChoose(char* out, char* expr1, char* expr2, char* expr3, char* expr4);
char* callTranspose(char* out, char* variableName, int row_count, int column_count);
char* callAdd(char* out, char* variableName1, char* variableName2, int row_count, int column_count);
char* callSubtract(char* out, char* variableName1, char* variableName2, int row_count, int column_count);
char* callMultiply(char* out, char* variableName1, char* variableName2, int row_count1, int column_count1, int row_count2, int column_count2);

extern int line_number;
extern struct Scalar scalars[];
extern struct Matrix matrices[];
extern int scalar_count;
extern int matrix_count;
extern char *terminals[];
extern struct Matrix* matrixListPointer;
extern int tempCount;
extern FILE *out_file;

int for_loop_open = 0;  // 0: There isn't a for loop currently open; 1: There is a single for loop currently open,
                              // 2: There is a double for loop currently open.

int rpn_size;  // When an expression is being converted to postfix notation, this variable stores the number of elements
               // in the postfix expression (it is different from the number of elements in the infix notation because
               // parentheses and commas aren't included in postfix notation).

char* eval(char **line, int size) {
    // Basically, this is the parent function that evaluates the right-hand side during an assignment. int size
    // is the number of elements in the right-hand side of the assignment. Note that size also contains the newline
    // character at the end of the line.
    if(strcmp(line[size - 1], "\n") != 0) {
        // If there is no newline character at the end of the line (i.e. when it is the last line in the .mat file), we
        // increment size by 1 to compensate for it.
        size++;
    }

    if(size == 1) {
        // When the line is blank
        return "\n";
    }

    int dimensions[2];

    // Scalar Declaration
    if(strcmp(line[0], "scalar") == 0) {
        if(size != 3) {
            error(line_number);
            return "error";
        }
        if(get_dimensions(line[1], dimensions) != NULL) {
            error(line_number);
        }
        char out[2048];
        return declaration(out, line[1], 0, 0);
    }

    // Vector declaration
    if(strcmp(line[0], "vector") == 0) {
        if(size != 6 || strcmp(line[2], "[") != 0 || strcmp(line[4], "]") != 0) {
            error(line_number);
            return "error";
        }
        if(strchr(line[3], (int) '.') != NULL) {
            error(line_number);
            return "error";
        }
        if(get_dimensions(line[1], dimensions) != NULL) {
            error(line_number);
        }
        char out[2048];
        int row_count = atoi(line[3]);
        return declaration(out, line[1], 1, row_count);
    }

    // Matrix declaration
    if(strcmp(line[0], "matrix") == 0) {
        if(size != 8 || strcmp(line[2], "[") != 0 || strcmp(line[4], ",") != 0 || strcmp(line[6], "]") != 0) {
            error(line_number);
            return "error";
        }
        if(strchr(line[3], (int) '.') != NULL || strchr(line[5], (int) '.') != NULL) {
            error(line_number);
            return "error";
        }
        if(get_dimensions(line[1], dimensions) != NULL) {
            error(line_number);
        }
        char out[2048];
        return declaration(out, line[1], atoi(line[5]), atoi(line[3]));
    }


    if(get_dimensions(line[0], dimensions) != NULL) {
        // The code enters this branch if the first symbol of the line is a variable. This indicates an assignment line.
        if(dimensions[0] == 0 && dimensions[1] == 0) {
            // Scalar assignment
            if (size < 4 || strcmp(line[1], "=") != 0) {
                error(line_number);
                return "error";
            }
            char *out[size - 3];
            char* translated = NULL;
            int *rhs_dims = typecheck(rpn(line, out, 2, size - 2), size - 3, &translated);
            if(rhs_dims[0] == 0 && rhs_dims[1] == 0) {
                char *final_line = (char*) calloc(2048, sizeof(char));
                return matrixAssignment(final_line, line[0], translated, 0, 0);
            } else {
                error(line_number);
            }
        } else {
            if(size >= 6 && strcmp(line[1], "=") == 0 && strcmp(line[2], "{") == 0 && strcmp(line[size - 2], "}") == 0) {
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

                char *array_literal = (char*) calloc(2048, sizeof(char));
                array_literal[0] = '{';
                for(int i = 3; i < size - 3; i++) {
                    strcat(array_literal, line[i]);
                    strcat(array_literal, ",");
                }
                strcat(array_literal, line[size - 3]);
                strcat(array_literal, "}");

                char *final_line = (char*) calloc(2048, sizeof(char));
                return matrixAssignment(final_line, line[0], array_literal, dimensions[0], dimensions[1]);

            } else {
                // Non-explicit matrix assignment or indexed assignment
                int equals_sign_location = -1;
                for (int i = 0; i < size; i++) {
                    if (strcmp(line[i], "=") == 0) {
                        equals_sign_location = i;
                        break;
                    }
                }
                if (equals_sign_location == -1) {
                    error(line_number);
                }

                char *out_lhs[equals_sign_location];
                char *translated_lhs = NULL;
                int *lhs_dims = typecheck(rpn(line, out_lhs, 0, equals_sign_location - 1), equals_sign_location, &translated_lhs);

                char *out_rhs[size - equals_sign_location - 2];
                char *translated_rhs = NULL;
                int *rhs_dims = typecheck(rpn(line, out_rhs, equals_sign_location + 1, size - 2), size - equals_sign_location - 2, &translated_rhs);

                if (lhs_dims[0] == rhs_dims[0] && lhs_dims[1] == rhs_dims[1]) {
                    char *final_line = (char *) calloc(2048, sizeof(char));
                    if(equals_sign_location == 1) {
                        // Non-explicit matrix assignment
                        return matrixAssignment(final_line, line[0], translated_rhs, dimensions[0], dimensions[1]);
                    } else {
                        // Indexed matrix assignment (e.g. M[2,1] = 7)
                        snprintf(final_line, 2048, "%s = %s;\n", translated_lhs, translated_rhs);
                        return final_line;
                    }
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
        if(size < 12 || strcmp(line[1], "(") != 0) {
            error(line_number);
        }
        for(int i = 0; i < 23; i++) {
            if(strcmp(line[2], terminals[i]) == 0) {
                error(line_number);
            }
        }
        char *temp = (char*) calloc(2048, sizeof(char));
        declaration(temp, line[2], 0, 0);
        if(strcmp(line[3], "in") == 0) {
            // Single for loop
            for_loop_open = 1;

            int start = 4;
            int ptr = 4;
            while(ptr < size && strcmp(line[ptr], ":") != 0) {
                ptr++;
            }
            if(ptr == size || ptr == start) {
                error(line_number);
            }
            char *expr1rpn[ptr - start];
            char *expr1translated = NULL;
            int *expr1dims = typecheck(rpn(line, expr1rpn, start, ptr - 1), ptr - start, &expr1translated);
            if(expr1dims[0] != 0 || expr1dims[1] != 0) {
                error(line_number);
            }
            start = ptr + 1;
            ptr = start;
            while(ptr < size && strcmp(line[ptr], ":") != 0) {
                ptr++;
            }
            if(ptr == size || ptr == start) {
                error(line_number);
            }
            char *expr2rpn[ptr - start];
            char *expr2translated = NULL;
            int *expr2dims = typecheck(rpn(line, expr2rpn, start, ptr - 1), ptr - start, &expr2translated);
            if(expr2dims[0] != 0 || expr2dims[1] != 0) {
                error(line_number);
            }
            start = ptr + 1;
            ptr = start;
            while(ptr < size && strcmp(line[ptr], ")") !=  0) {
                ptr++;
            }
            if(ptr == size || ptr == start) {
                error(line_number);
            }
            char *expr3rpn[ptr - start];
            char *expr3translated = NULL;
            int *expr3dims = typecheck(rpn(line, expr3rpn, start, ptr - 1), ptr - start, &expr3translated);
            if(expr3dims[0] != 0 || expr3dims[1] != 0) {
                error(line_number);
            }
            if(ptr != size - 3 || strcmp(line[ptr + 1], "{") != 0) {
                error(line_number);
            }

            char *final_line = (char*) calloc(2048, sizeof(char));
            return singleForLoop(final_line, line[2], expr1translated, expr2translated, expr3translated);

        } else if(strcmp(line[3], ",") == 0) {
            // Double for loop
            for_loop_open = 2;

            for(int i = 0; i < 23; i++) {
                if(strcmp(line[4], terminals[i]) == 0) {
                    error(line_number);
                }
            }
            char *temp = (char*) calloc(2048, sizeof(char));
            declaration(temp, line[4], 0, 0);
            if(strcmp(line[5], "in") != 0) {
                error(line_number);
            }
            int start = 6;
            int ptr = 6;
            while(ptr < size && strcmp(line[ptr], ":") != 0) {
                ptr++;
            }
            if(ptr == size || ptr == start) {
                error(line_number);
            }
            char *expr1rpn[ptr - start];
            char *expr1translated = NULL;
            int *expr1dims = typecheck(rpn(line, expr1rpn, start, ptr - 1), ptr - start, &expr1translated);
            if(expr1dims[0] != 0 || expr1dims[1] != 0) {
                error(line_number);
            }
            start = ptr + 1;
            ptr = start;
            while(ptr < size && strcmp(line[ptr], ":") != 0) {
                ptr++;
            }
            if(ptr == size || ptr == start) {
                error(line_number);
            }
            char *expr2rpn[ptr - start];
            char *expr2translated = NULL;
            int *expr2dims = typecheck(rpn(line, expr2rpn, start, ptr - 1), ptr - start, &expr2translated);
            if(expr2dims[0] != 0 || expr2dims[1] != 0) {
                error(line_number);
            }
            start = ptr + 1;
            ptr = start;
            while(ptr < size && strcmp(line[ptr], ",") !=  0) {
                ptr++;
            }
            if(ptr == size || ptr == start) {
                error(line_number);
            }
            char *expr3rpn[ptr - start];
            char *expr3translated = NULL;
            int *expr3dims = typecheck(rpn(line, expr3rpn, start, ptr - 1), ptr - start, &expr3translated);
            if(expr3dims[0] != 0 || expr3dims[1] != 0) {
                error(line_number);
            }
            start = ptr + 1;
            ptr = start;
            while(ptr < size && strcmp(line[ptr], ":") != 0) {
                ptr++;
            }
            if(ptr == size || ptr == start) {
                error(line_number);
            }
            char *expr4rpn[ptr - start];
            char *expr4translated = NULL;
            int *expr4dims = typecheck(rpn(line, expr4rpn, start, ptr - 1), ptr - start, &expr4translated);
            if(expr4dims[0] != 0 || expr4dims[1] != 0) {
                error(line_number);
            }
            start = ptr + 1;
            ptr = start;
            while(ptr < size && strcmp(line[ptr], ":") != 0) {
                ptr++;
            }
            if(ptr == size || ptr == start) {
                error(line_number);
            }
            char *expr5rpn[ptr - start];
            char *expr5translated = NULL;
            int *expr5dims = typecheck(rpn(line, expr5rpn, start, ptr - 1), ptr - start, &expr5translated);
            if(expr5dims[0] != 0 || expr5dims[1] != 0) {
                error(line_number);
            }
            start = ptr + 1;
            ptr = start;
            while(ptr < size && strcmp(line[ptr], ")") != 0) {
                ptr++;
            }
            if(ptr == size || ptr == start) {
                error(line_number);
            }
            char *expr6rpn[ptr - start];
            char *expr6translated = NULL;
            int *expr6dims = typecheck(rpn(line, expr6rpn, start, ptr - 1), ptr - start, &expr6translated);
            if(expr6dims[0] != 0 || expr6dims[1] != 0) {
                error(line_number);
            }
            if(ptr != size - 3 || strcmp(line[ptr + 1], "{") != 0) {
                error(line_number);
            }

            char *final_line = (char*) calloc(2048, sizeof(char));
            return doubleForLoop(final_line, line[2], line[4], expr1translated, expr2translated, expr3translated, expr4translated, expr5translated, expr6translated);

        } else {
            error(line_number);
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
            matrix_count--;
            matrixListPointer--;
            return "}\n";
        } else if(for_loop_open == 2) {
            for_loop_open = 0;
            matrix_count -= 2;
            matrixListPointer -= 2;
            return "}}\n";
        } else{
            error(line_number);
            return "error";
        }
    }

    if(strcmp(line[0], "print") == 0) {
        // Print statement
        if(size < 5 || strcmp(line[1], "(") != 0 || strcmp(line[size - 2], ")") != 0) {
            error(line_number);
        }
        char *expr_rpn[size - 4];
        char *expr_translated = NULL;
        int *expr_dims = typecheck(rpn(line, expr_rpn, 2, size - 3), size - 4, &expr_translated);
        char *final_line = (char*) calloc(2048, sizeof(char));
        return callPrintId(final_line, expr_translated, expr_dims[0], expr_dims[1]);
    }

    if(strcmp(line[0], "printsep") == 0) {
        // Printsep statement
        if(size != 4 || strcmp(line[1], "(") != 0 || strcmp(line[2], ")") != 0) {
            error(line_number);
            return "error";
        }
        return printSep();
    }

    // If the line doesn't match any of the patterns above, then the line can't be legal in the Matlang language.
    error(line_number);
    return "error";

}

int *typecheck(char **line, int size, char **ptranslated) {
    // This function translates a given expression from Matlang to C. During translation, it also does typechecking.
    // If a type error is encountered, the error() function is called and translation stops. Otherwise, the translated
    // expression is stored in char *translated. char **line is the list of tokens of the expression in Reverse Polish
    // Notation. At the end of the function, the dimensions of the expression are returned, and the translated version
    // of the expression is stored inside char **ptranslated.

    // The dimensions of the expressions are stored in this stack:
    int (*stack)[2] = (int(*)[2]) calloc(size, sizeof(int[2]));
    int (*stack_begin)[2] = stack;

    // The postfix versions of the expressions are stored in this stack:
    char **expr_stack = (char**) calloc(size, sizeof(char*));
    char **expr_stack_begin = expr_stack;

    for(int i = 0; i < rpn_size; i++) {
        if(strcmp(line[i], "*") == 0) {
            // Multiplication
            if(stack - stack_begin < 2) {
                error(line_number);
            }
            stack--; int r2 = (*stack)[0]; int c2 = (*stack)[1];
            stack--; int r1 = (*stack)[0]; int c1 = (*stack)[1];
            if(r1 == 0 && c1 == 0) {
                (*stack)[0] = r2; (*stack)[1] = c2; stack++;
            } else if(c1 == r2) {
                (*stack)[0] = r1; (*stack)[1] = c2; stack++;
            } else {
                error(line_number);
            }
            expr_stack--; char* expr2 = strdup(*expr_stack);
            expr_stack--; char* expr1 = strdup(*expr_stack);
            callMultiply(*expr_stack, expr1, expr2, r1, c1, r2, c2);
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
            expr_stack--; char* expr2 = strdup(*expr_stack);
            expr_stack--; char* expr1 = strdup(*expr_stack);
            if(strcmp(line[i], "+") == 0) {
                // Addition
                callAdd(*expr_stack, expr1, expr2, r1, c1);
            } else {
                // Subtraction
                callSubtract(*expr_stack, expr1, expr2, r1, c1);
            }
            expr_stack++;

        } else if(strcmp(line[i], "tr") == 0) {
            // Transposition
            if(stack - stack_begin < 1) {
                error(line_number);
            }
            stack--; int r1 = (*stack)[0]; int c1 = (*stack)[1];
            (*stack)[0] = c1; (*stack)[1] = r1; stack++;
            expr_stack--; char* expr = strdup(*expr_stack);
            callTranspose(*expr_stack, expr, r1, c1);
            expr_stack++;

        } else if(strcmp(line[i], "sqrt") == 0) {
            // Square root
            if(stack - stack_begin < 1) {
                error(line_number);
            }
            stack--; int r1 = (*stack)[0]; int c1 = (*stack)[1];
            if(r1 == 0 && c1 == 0) {
                (*stack)[0] = r1; (*stack)[1] = c1; stack++;
            } else {
                error(line_number);
            }
            expr_stack--; char* expr = strdup(*expr_stack);
            callSqrt(*expr_stack, expr);
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
            if(r1 == 0 && r2 == 0 && r3 == 0 && r4 == 0 && c1 == 0 && c2 == 0 && c3 == 0 && c4 == 0) {
                (*stack)[0] = 0; (*stack)[1] = 0; stack++;
            } else {
                error(line_number);
            }
            expr_stack--; char* expr4 = strdup(*expr_stack);
            expr_stack--; char* expr3 = strdup(*expr_stack);
            expr_stack--; char* expr2 = strdup(*expr_stack);
            expr_stack--; char* expr1 = strdup(*expr_stack);
            callChoose(*expr_stack, expr1, expr2, expr3, expr4);
            expr_stack++;

        } else if(isdigit((int) line[i][0]) || line[i][0] == '.') {
            // Number
            (*stack)[0] = 0; (*stack)[1] = 0; stack++;
            *expr_stack = strdup(line[i]); expr_stack++;

        } else if(strcmp(line[i], "[") == 0 || strcmp(line[i], "]") == 0) {
            // Bracket
            *expr_stack = strdup(line[i]); expr_stack++;

        } else {
            // Variable or error
            int dim[2];
            if(get_dimensions(line[i], dim) == NULL) {
                error(line_number);
            }

            if(expr_stack > expr_stack_begin && strcmp(*(expr_stack - 1), "]") == 0) {
                // Matrix indexing
                if(expr_stack - expr_stack_begin >= 3 && strcmp(*(expr_stack - 3), "[") == 0) {
                    // Single argument matrix indexing (i.e. vector indexing)
                    if(dim[1] != 1 || dim[0] == 0) {
                        error(line_number);
                    }
                    stack--; int r1 = (*stack)[0]; int c1 = (*stack)[1];
                    if(r1 != 0 || c1 != 0) {
                        error(line_number);
                    }
                    (*stack)[0] = 0; (*stack)[1] = 0; stack++;
                    expr_stack -= 2; char *expr = strdup(*expr_stack); expr_stack--;
                    snprintf(*expr_stack, 2048, "%s[(int) floor(subtractS(%s, 1))][0]", line[i], expr);
                    expr_stack++;

                } else if(expr_stack - expr_stack_begin >= 4 && strcmp(*(expr_stack - 4), "[") == 0) {
                    // Double argument matrix indexing
                    if(dim[0] == 0 || dim[1] == 0) {
                        error(line_number);
                    }
                    stack--; int r2 = (*stack)[0]; int c2 = (*stack)[1];
                    stack--; int r1 = (*stack)[0]; int c1 = (*stack)[1];
                    if(r1 != 0 || r2 != 0 || c1 != 0 || c2 != 0) {
                        error(line_number);
                    }
                    (*stack)[0] = 0; (*stack)[1] = 0; stack++;
                    expr_stack -= 2; char *expr2 = strdup(*expr_stack);
                    expr_stack--; char *expr1 = strdup(*expr_stack); expr_stack--;
                    snprintf(*expr_stack, 2048, "%s[(int) floor(subtractS(%s, 1))][(int) floor(subtractS(%s, 1))]", line[i], expr1, expr2);
                    expr_stack++;

                } else {
                    error(line_number);
                }
            } else {
                (*stack)[0] = dim[0]; (*stack)[1] = dim[1]; stack++;
                *expr_stack = strdup(line[i]); expr_stack++;
            }
        }
    }

    // When translation / typechecking ends, the stack should contain exactly 1 expression (which is the final
    // translated version of the starting expression). If this is not the case, then there is an error.
    if(stack - stack_begin != 1) {
        error(line_number);
    }

    int *out = (int*) calloc(2, sizeof(int));
    out[0] = (*stack_begin)[0]; out[1] = (*stack_begin)[1];
    *ptranslated = strdup(*expr_stack_begin);
    return out;
}

int *get_dimensions(char *name, int *dimensions) {
    // This function returns the dimensions of a variable if it exists, or NULL otherwise.
    for(int i = 0; i < scalar_count; i++) {
        if(strcmp(name, scalars[i].name) == 0) {
            dimensions[0] = 0;
            dimensions[1] = 0;
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

char **rpn(char **line, char **out, int start_index, int end_index) {
    // This function uses a modified version of Dijkstra's Shunting-yard Algorithm to convert a mathematical expression
    // to Reverse Polish Notation. The algorithm is modified in a way that allows multiple-argument functions (in our
    // case, matrix indexing) to be present. int start_index and int end_index specify where the expression to be
    // converted is situated inside the line.
    rpn_size = end_index - start_index + 1;
    int out_index = 0;
    char **operator_stack = (char**) calloc(end_index - start_index + 1, sizeof(char*));
    char **operator_stack_begin = operator_stack;
    for(int i = start_index; i <= end_index; i++) {
        char *token = line[i];
        int dimensions[2];
        if(isdigit((int) token[0]) || token[0] == '.' ||
        (get_dimensions(token, dimensions) != NULL && (i == end_index || strcmp(line[i + 1], "[") != 0))) {
            // If the token is a number or a variable that is not being indexed
            out[out_index] = token;
            out_index++;
        } else if(strcmp(token, "tr") == 0 || strcmp(token, "sqrt") == 0 || strcmp(token, "choose") == 0 ||
                (get_dimensions(token, dimensions) != NULL && i < end_index && strcmp(line[i + 1], "[") == 0)) {
            // If the token is a function (this also includes a variable that is being indexed)
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
            rpn_size--;
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
            rpn_size--;
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
            // If the token is a comma
            rpn_size--;
            while(operator_stack != operator_stack_begin && (strcmp(*(operator_stack - 1), "*") == 0 || strcmp(*(operator_stack - 1), "-") == 0 ||
            strcmp(*(operator_stack - 1), "+") == 0)) {
                operator_stack--;
                out[out_index] = *operator_stack;
                out_index++;
            }
        } else {
            // Unidentified token
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
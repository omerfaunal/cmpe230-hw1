#include <stdio.h>
#include <string.h>
#include "structs.h"

extern struct Scalar scalars[];
extern struct Scalar* scalarListPointer;
extern struct Matrix* matrixListPointer;
extern int scalar_count;
extern int matrix_count;
extern int line_number;
int tempCount = 0;

void error(int line);

/*
 * Given a variableName and type, according to type this function assigns that variable in out.c
 */
char* declaration(char* out, char* variableName, int columnCount, int rowCount) {
    struct Matrix matrix;
    matrix.name = strdup(variableName);
    matrix.column_count = columnCount;
    matrix.row_count = rowCount;
    /*
     * If rowCount of variable is 0, according to our implementation this means variable is scalar
     * invokes float assigning code in out.c
     */
    if(rowCount == 0) {
        snprintf(out, 2048, "float %s = 0;\n", variableName);
    } else {
        /* If variable is matrix */
        char loop_variable[3] = "i";
        if (strcmp(variableName, "i") == 0) {
            strcat(loop_variable, "_");
        }
        /* Assign matrix as float pointer to pointer and allocate matrix in memory */
        snprintf(out, 2048, "float** %s = (float**) calloc(%d, sizeof(float*));\n"
                            "    for(int %s = 0; %s < %d; %s++) {\n"
                            "        %s[%s] = (float*) calloc(%d, sizeof(float));\n"
                            "    }\n", variableName, rowCount, loop_variable, loop_variable, rowCount, loop_variable,
                 variableName, loop_variable, columnCount);
    }
    /* Add this matrix to matrix struct list */
    *matrixListPointer = matrix;
    matrixListPointer += 1;
    matrix_count++;

    return out;
}

/*
 * Given a variableName and expressions, assigns that expressions to variable in out.c
 */
char* assignment(char* out, char* variableName, char* values, int rowCount, int columnCount) {
    /* If rowCounts is 0 according to out implementation this means variable is scalar, so invoke scalar assignment */
    if(rowCount == 0 && columnCount == 0) {
        snprintf(out, 2048, "%s = %s;\n", variableName, values);
    } else {
        /* If variable is matrix */
        char loop_variable1[3] = "i";
        while (strcmp(variableName, loop_variable1) == 0) {
            strcat(loop_variable1, "_");
        }
        char loop_variable2[3] = "j";
        while (strcmp(variableName, loop_variable2) == 0) {
            strcat(loop_variable2, "_");
        }

        /* Create a temp matrix, put the values to it and assign pre-allocated memory to given values */
        if (values[0] == '{') {
            snprintf(out, 2048, "float _temp%d[%d][%d] = %s;\n"
                                "for(int %s = 0; %s < %d; %s++){\n"
                                "    for(int %s = 0; %s < %d; %s++){\n"
                                "        %s[%s][%s] = _temp%d[%s][%s];\n"
                                "    }\n"
                                "}\n", tempCount, rowCount, columnCount, values,
                     loop_variable1, loop_variable1, rowCount, loop_variable1,
                     loop_variable2, loop_variable2, columnCount, loop_variable2,
                     variableName, loop_variable1, loop_variable2, tempCount, loop_variable1, loop_variable2);
        } else {
            snprintf(out, 2048, "float **_temp%d = %s;\n"
                                "for(int %s = 0; %s < %d; %s++){\n"
                                "    for(int %s = 0; %s < %d; %s++){\n"
                                "        %s[%s][%s] = _temp%d[%s][%s];\n"
                                "    }\n"
                                "}\n", tempCount, values,
                     loop_variable1, loop_variable1, rowCount, loop_variable1,
                     loop_variable2, loop_variable2, columnCount, loop_variable2,
                     variableName, loop_variable1, loop_variable2, tempCount, loop_variable1, loop_variable2);
        }

        tempCount += 1;
    }

    return out;
}
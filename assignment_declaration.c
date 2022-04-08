#include <stdio.h>
#include <string.h>
#include "structs.h"

extern struct Scalar scalars[];
extern struct Scalar* scalarListPointer;
extern struct Matrix* matrixListPointer;
extern short int scalar_count;
extern short int matrix_count;
extern int line_number;
int tempCount = 0;

void error(int line);

char* declaration(char* out, char* variableName, int columnCount, int rowCount) {
    struct Matrix matrix;
    matrix.name = strdup(variableName);
    matrix.column_count = columnCount;
    matrix.row_count = rowCount;
    if(rowCount == 0) {
        snprintf(out, 2048, "float %s = 0;\n", variableName);
    } else {
        char loop_variable[3] = "i";
        if (strcmp(variableName, "i") == 0) {
            strcat(loop_variable, "_");
        }
        snprintf(out, 2048, "float** %s = (float**) calloc(%d, sizeof(float*));\n"
                            "    for(int %s = 0; %s < %d; %s++) {\n"
                            "        %s[%s] = (float*) calloc(%d, sizeof(float));\n"
                            "    }\n", variableName, rowCount, loop_variable, loop_variable, rowCount, loop_variable,
                 variableName, loop_variable, columnCount);
    }
    *matrixListPointer = matrix;
    matrixListPointer += 1;
    matrix_count++;

    return out;
}

char* matrixAssignment(char* out, char* variableName, char* values, int rowCount, int columnCount) {
    if(rowCount == 0 && columnCount == 0) {
        snprintf(out, 2048, "%s = %s;\n", variableName, values);
    } else {
        char loop_variable1[3] = "i";
        while (strcmp(variableName, loop_variable1) == 0) {
            strcat(loop_variable1, "_");
        }
        char loop_variable2[3] = "j";
        while (strcmp(variableName, loop_variable2) == 0) {
            strcat(loop_variable2, "_");
        }

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
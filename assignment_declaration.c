#include <stdio.h>
#include <string.h>
#include "structs.h"

#define MAX_CHAR 256

extern struct Scalar scalars[];
extern struct Scalar* scalarListPointer;
extern struct Matrix* matrixListPointer;
extern short int scalar_count;
extern short int matrix_count;
extern int line_number;

void error(int line);

//char* scalarValueDeclaration(char* out, char* variableName, float value) {
//    TODO: Declare scalars at matrices with dimension 0x0.
//    snprintf(out, MAX_CHAR + 50, "float %s = %f;\n", variableName, value);
//    struct Scalar scalar;
//    scalar.name = strdup(variableName);
//    scalar.value = value;
//    *scalarListPointer = scalar;
//    scalarListPointer += 1;
//    scalar_count++;
//    return out;
//}

char* assignment(char* out, char** tokens, int tokens_size) {
    for(int i = 0; i < tokens_size; i++) {
        strcat(out, tokens[i]);
    }
    return out;
}

char* declaration(char* out, char* variableName, int columnCount, int rowCount) {
    if(columnCount < 0 || rowCount < 0){
        error(line_number);
        return out;
    }
    struct Matrix matrix;
    matrix.name = strdup(variableName);
    matrix.column_count = columnCount;
    matrix.row_count = rowCount;
    if(rowCount == 0) {
        rowCount++;
        columnCount++;
    }
    char loop_variable[3] = "i";
    if(strcmp(variableName, "i") == 0) {
        strcat(loop_variable, "_");
    }
    snprintf(out, 512, "int** %s = (int**) calloc(%d, sizeof(int*));\n"
                       "    for(int %s = 0; %s < %d; %s++) {\n"
                       "        %s[%s] = (int*) calloc(%d, sizeof(int));\n"
                       "    }\n", variableName, rowCount, loop_variable, loop_variable, rowCount, loop_variable,
                       variableName, loop_variable, columnCount);
    *matrixListPointer = matrix;
    matrixListPointer += 1;
    matrix_count++;
    return out;
}

char* matrixAssignment(char* out, char* variableName, char* values, int rowCount, int columnCount) {
    // TODO: This should check whether rowCount == columnCount == 0, and if so then do scalar assignment
    char loop_variable1[3] = "i";
    while(strcmp(variableName, loop_variable1) == 0 || strcmp(variableName, loop_variable1) == 0) {
        strcat(loop_variable1, "_");
    }
    char loop_variable2[3] = "j";
    while(strcmp(variableName, loop_variable2) == 0 || strcmp(variableName, loop_variable2) == 0) {
        strcat(loop_variable2, "_");
    }

    snprintf(out, 256, "int** _temp_ = %s;\n"
                       "for(int %s = 0; %s < %d; %s++){\n"
                       "    for(int %s = 0; %s < %d; %s++){\n"
                       "        %s[%s][%s] = _temp_[%s][%s];\n"
                       "    }\n"
                       "}\n", values, loop_variable1, loop_variable1, rowCount, loop_variable1, loop_variable2, loop_variable2,
                       columnCount, loop_variable2, variableName, loop_variable1, loop_variable2, loop_variable1, loop_variable2);
    return out;
}
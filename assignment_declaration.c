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

char* scalarValueDeclaration(char* out, char* variableName, float value) {
    snprintf(out, MAX_CHAR + 50, "float %s = %f;\n", variableName, value);
    struct Scalar scalar;
    scalar.name = variableName;
    scalar.value = value;
    *scalarListPointer = scalar;
    scalarListPointer += 1;
    scalar_count++;
    return out;
}

char* scalarValueAssignment(char* out, char* variableName, float value) {
    for(int i = 0; i < 256; i++){
        struct Scalar currentScalar = scalars[i];
        if(strcmp(currentScalar.name, variableName) == 0){
            currentScalar.value = value;
            scalars[i] = currentScalar;
            return out;
        }
    }
    //Gives an error if variable is not declared
    error(line_number);
    snprintf(out, 80, "%s = %f;\n", variableName, value);
    return out;
}

char* matrixDeclaration(char* out, char* variableName, int columnCount, int rowCount) {
    // TODO: Check if dimensions are positive.
    struct Matrix matrix;
    matrix.name = variableName;
    matrix.column_count = columnCount;
    matrix.row_count = rowCount;
    snprintf(out, MAX_CHAR + 50, "float %s[%d][%d];\n", variableName, matrix.row_count, matrix.column_count);
    *matrixListPointer = matrix;
    matrixListPointer += 1;
    matrix_count++;
    return out;
}

//TODO
char* matrixAssignment(char* out, char* variableName, int arraySize) {
    snprintf(out, 256, "for(int i = 0; i < %d; i++) {%s[i] = values[i];}\n", arraySize, variableName);
    return out;
}
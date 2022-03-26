#include <stdio.h>
#include <string.h>
#include "structs.h"

#define MAX_CHAR 256

extern struct Scalar scalars[];
extern struct Scalar* scalarListPointer;
extern struct Matrix* matrixListPointer;

void error(int line);

char* scalarValueDeclaration(char* out, char* variableName, float value) {
    snprintf(out, MAX_CHAR, "float %s = %f;\n", variableName, value);
    struct Scalar scalar;
    scalar.name = variableName;
    scalar.value = value;
    *scalarListPointer = scalar;
    scalarListPointer += 1;
    return out;
}

char* scalarValueAssignment(char* out, int lineNo, char* variableName, float value) {
    for(int i = 0; i < 256; i++){
        struct Scalar currentScalar = scalars[i];
        if(strcmp(currentScalar.name, variableName) == 0){
            currentScalar.value = value;
            scalars[i] = currentScalar;
            return out;
        }
    }
    //Gives an error if variable is not declared
    error(lineNo);
    snprintf(out, 80, "%s = %f;\n", variableName, value);
    return out;
}

char* matrixDeclaration(char* out, char* variableName, int columnCount, int rowCount) {
    struct Matrix matrix;
    matrix.name = variableName;
    matrix.column_count = columnCount;
    matrix.row_count = rowCount;
    snprintf(out, 80, "float %s[%d][%d];\n", variableName, matrix.row_count, matrix.column_count);
    *matrixListPointer = matrix;
    scalarListPointer += 1;
    return out;
}

//TODO
char* matrixAssignment(char* out, char* variableName, int arraySize) {
    snprintf(out, 256, "for(int i = 0; i < %d; i++) {%s[i] = values[i];}\n", arraySize, variableName);
    return out;
}
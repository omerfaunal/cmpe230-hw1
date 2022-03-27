#include <math.h>
#include <stdio.h>
#include <string.h>
#include "structs.h"

extern struct Scalar scalars[];

void error(int line);

char* printSep() {
    return "printf(\"%s\", \"----------\");";
}

char* printId(char* out, float id) {
    snprintf(out, 80, "printf(\"%s\", \"%f\")","%f;", id);
    return out;
}

//Careful! This functions only return the sqrt value and changes the scalar value in this program.
//After using calculateSqrt function, make sure that you also use scalarValueAssignment function.
float calculateSqrt(int lineNo, char* variableName) {
    float value;
    for(int i = 0; i < 256; i++){
        struct Scalar currentScalar = scalars[i];
        if(strcmp(currentScalar.name, variableName) == 0){
            value = currentScalar.value;
            if(value < 0){
                error(lineNo);
                return -1;
            }
            currentScalar.value = sqrtf(value);
            scalars[i] = currentScalar;
            return sqrtf(value);
        }
    }
    error(lineNo);
    return -1;
}

char* singleForLoop(char* out, char* expr1, char* expr2, char* expr3) {
    snprintf(out, 256, "for(int i = %s; i < %s; i += %s) {\n", expr1, expr2, expr3);
    return out;
}

char* doubleForLoop(char* out, char* expr1, char* expr2, char* expr3, char* expr4, char* expr5, char* expr6) {
    snprintf(out, 256, "for(int i = %s; i < %s; i += %s ) { \n for(int j = %s; j < %s; j += %s )", expr1, expr2, expr3, expr4, expr5, expr6);
    return out;
}
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
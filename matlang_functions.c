#include <stdio.h>
#include "structs.h"

extern struct Scalar scalars[];

void error(int line);

/*
 * Function to print separation like ------------
 * This function will be pushed to out.c
*/
char* printSep() {
    return "printf(\"------------\\n\");\n";
}

/*
 * Declaration of printId function
 * Given a matrix, this function prints every element of matrix line by line
 * This function will be pushed to out.c
 */
char *declarePrintId(char *out) {
    snprintf(out, 2048, "void printId(float **variable, int row_count, int column_count){\n"
           "for(int i = 0; i < row_count; i++){\n"
           "    for(int j = 0; j < column_count; j++) {\n"
           "        if(fabs(variable[i][j] - round(variable[i][j])) < 0.0000005) {\n"
           "        printf(\"%s \",(int) round(variable[i][j]));\n"
           "       } \n"
           "       else {\n"
           "        printf(\"%s \",(float) variable[i][j]);\n"
           "       }"
           "    }\n"
           "    printf(\"\\n\");\n"
           "}\n"
           "}\n", "%d", "%f");
    return out;
}

/*
 * Declaration of scalar printId function
 * Simply prints scalar, if scalar is integer uses integer printing function else uses float integer function
 * This function will be pushed to out.c
*/
char *declarePrintIdS(char *out) {
    snprintf(out, 2048, "void printIdS(float variable){\n"
                        "    if(fabs(variable - round(variable)) < 0.0000005) {\n"
                        "        printf(\"%s\\n\",(int) round(variable));\n"
                        "    } \n"
                        "    else {\n"
                        "        printf(\"%s\\n\",(float) variable);\n"
                        "    }"
                        "}\n", "%d", "%f");
    return out;
}

/*
 * Function to invoke pre declared printId function in out.c
*/
char* callPrintId(char* out, char* variableName, int row_count, int column_count) {
    if(row_count == 0 && column_count == 0) {
        snprintf(out, 2048, "printIdS(%s);\n", variableName);
    } else {
        snprintf(out, 2048, "printId(%s, %d, %d);\n", variableName, row_count, column_count);
    }
    return out;
}

/*
 * This function takes and id and three expression and creates for loop in the following structure
 * for(float id = expr1; id < expr2; id += expr3)
 */
char* singleForLoop(char* out,char* id, char* expr1, char* expr2, char* expr3) {
    snprintf(out, 2048, "for(float %s = %s; %s <= %s; %s += %s) {\n",id, expr1,id, expr2,id, expr3);
    return out;
}

/*
 * This function creates a two for loop inside like.
 * Structure is same as single for loop
 */
char* doubleForLoop(char* out,char* id1, char* id2, char* expr1, char* expr2, char* expr3, char* expr4, char* expr5, char* expr6) {
    snprintf(out, 2048, "for(float %s = %s; %s <= %s; %s += %s ) { \n for(float %s = %s; %s <= %s; %s += %s ) {\n",
             id1, expr1, id1, expr2, id1, expr3, id2, expr4, id2, expr5, id2, expr6);
    return out;
}

/*
 * Declaration function for square root function.
 * This function will be pushed to out.c
 */
char* declareSqrtFunction(char* out) {
    snprintf(out, 2048, "float sqrt_(float value) {\n"
                        "   return sqrtf(value);\n"
                        "}\n");
    return out;
}

/*
 * Declaration function for choose function.
 * Choose function takes 4 input and if expr is 1 returns expr2, if expr1 < returns expr3 and else returns expr4
 * This function will be pushed to out.c
 */
char* declareChooseFunction(char* out) {
    snprintf(out, 2048, "float choose(float expr1, float expr2, float expr3, float expr4) {\n"
                        "    float out;\n"
                        "    if(fabs(expr1) < 0.0000005) {\n"
                        "        out = expr2;\n"
                        "    } else if (expr1 > 0) {\n"
                        "        out = expr3;\n"
                        "    } else {\n"
                        "        out = expr4;\n"
                        "    }\n"
                        "    return out;\n"
                        "}\n");
    return out;
}

/*
 * Declaration function for choose function.
 * Takes a matrix as an argument and transpose that matrix.
 * This function will be pushed to out.c
 */
char* declareTransposeFunction(char* out) {
    snprintf(out, 2048, "float** transpose(float** arr, int row_count, int column_count) {\n"
                       "   float** outArr = (float**) calloc(column_count, sizeof(float*));\n"
                       "   for(int i = 0; i < column_count; i++) {\n"
                       "       outArr[i] = (float*) calloc(row_count, sizeof(float));\n"
                       "       for(int j = 0; j < row_count; j++) {\n"
                       "           outArr[i][j] = arr[j][i];\n"
                       "       }\n"
                       "   }    return outArr;\n"
                       "}\n");
    return out;
}

/*
 * Declaration function for matrix-wise + operator.
 * Simply add two matrix cell by cell, and returns the result matrix.
 * This function will be pushed to out.c
 */
char* declareAddFunction(char* out) {
    snprintf(out, 2048, "float** add(float** arr1, float** arr2, int row_count, int column_count) {\n"
                        "    float** outArr = (float**) calloc(row_count, sizeof(float*));\n"
                        "    for(int i = 0; i < row_count; i++) {\n"
                        "        outArr[i] = (float*) calloc(column_count, sizeof(float));\n"
                        "        for(int j = 0; j < column_count; j++){\n"
                        "            outArr[i][j] = arr1[i][j] + arr2[i][j];\n"
                        "        }\n"
                        "    }\n"
                        "    return outArr;\n"
                        "}\n");
    return out;
}

/*
 * Declaration function for scalar + operator.
 * Simply add two scalar and returns the sum.
 * This function will be pushed to out.c
 */
char *declareAddFunctionS(char *out) {
    snprintf(out, 2048, "float addS(float val1, float val2) {\n"
                        "    return val1 + val2;\n"
                        "}\n");
    return out;
}

/*
 * Declaration function for matrix-wise - operator.
 * Simply subtract two matrix cell by cell, and returns the result matrix.
 * This function will be pushed to out.c
 */
char* declareSubtractFunction(char* out) {
    snprintf(out, 2048, "float** subtract(float** arr1, float** arr2, int row_count, int column_count) {\n"
                        "    if(row_count == 0) {\n"
                        "        float** outArr = (float**) calloc(1, sizeof(float*));\n"
                        "        outArr[0] = (float*) calloc(1, sizeof(float));\n"
                        "        outArr[0][0] = arr1[0][0] - arr2[0][0];\n"
                        "        return outArr;\n"
                        "    }\n"
                        "    float** outArr = (float**) calloc(row_count, sizeof(float*));\n"
                        "    for(int i = 0; i < row_count; i++) {\n"
                        "        outArr[i] = (float*) calloc(column_count, sizeof(float));\n"
                        "        for(int j = 0; j < column_count; j++){\n"
                        "            outArr[i][j] = arr1[i][j] - arr2[i][j];\n"
                        "        }\n"
                        "    }\n"
                        "    return outArr;\n"
                        "}\n");
    return out;
}

/*
 * Declaration function for scalar - operator.
 * Simply subtract two scalar and returns the sum.
 * This function will be pushed to out.c
 */
char *declareSubtractFunctionS(char *out) {
    snprintf(out, 2048, "float subtractS(float val1, float val2) {\n"
                        "   return val1 - val2;\n"
                        "}\n");
    return out;
}

/*
 * Declaration function for matrix-matrix * operator.
 * Simply multiply two matrix cell by cell, and returns the result matrix.
 * This function will be pushed to out.c
 */
char* declareMultiplyFunction(char* out) {
    snprintf(out, 2048, "float** multiply(float** arr1, float** arr2, int row_count1, int column_count1, int row_count2, int column_count2) {\n"
                        "    float** outArr3 = (float**) calloc(row_count1, sizeof(float*));\n"
                        "    for (int i = 0; i < row_count1; ++i) {\n"
                        "        outArr3[i] = (float*) calloc(column_count2, sizeof(float));\n"
                        "        for (int j = 0; j < column_count2; ++j) {\n"
                        "            for (int k = 0; k < column_count1; ++k) {\n"
                        "                outArr3[i][j] += arr1[i][k] * arr2[k][j];\n"
                        "            }\n"
                        "        }\n"
                        "     }\n"
                        "    return outArr3;\n"
                        "}\n");
    return out;
}

/*
 * Declaration function for scalar-scalar * operator.
 * Simply multiply two scalar and returns the sum.
 * This function will be pushed to out.c
 */
char *declareMultiplyFunctionSS(char *out) {
    snprintf(out, 2048, "float multiplySS(float val1, float val2) {\n"
                        "   return val1 * val2;\n"
                        "}\n");
    return out;
}

/*
 * Declaration function for scalar-matrix * operator.
 * Simply multiply every cell of the matrix with the given scalar, and returns the result matrix.
 * This function will be pushed to out.c
 */
char *declareMultiplyFunctionSM(char *out) {
    snprintf(out, 2048, "float **multiplySM(float val, float **matrix, int r, int c) {\n"
                        "    float** outArr = (float**) calloc(r, sizeof(float*));\n"
                        "    for(int i = 0; i < r; i++) {\n"
                        "        outArr[i] = (float*) calloc(c, sizeof(float));\n"
                        "        for(int j = 0; j < c; j++){\n"
                        "            outArr[i][j] = matrix[i][j] * val;\n"
                        "        }\n"
                        "    }\n"
                        "    return outArr;\n"
                        "}\n");
    return out;
}

/*
 * Function to invoke function in the out.c.
 */
char* callSqrt(char* out, char* variableName) {
    snprintf(out, 2048, "sqrt_(%s)", variableName);
    return out;
}

/*
 * Function to invoke function in the out.c.
 */
char* callChoose(char* out, char* expr1, char* expr2, char* expr3, char* expr4) {
    snprintf(out, 2048, "choose(%s, %s, %s, %s)", expr1, expr2, expr3, expr4);
    return out;
}

/*
 * Function to invoke function in the out.c.
 */
char* callTranspose(char* out, char* variableName, int row_count, int column_count) {
    if(row_count == 0 && column_count == 0) {
        snprintf(out, 2048, variableName);
    } else {
        snprintf(out, 2048, "transpose(%s, %d, %d)", variableName, row_count, column_count);
    }
    return out;
}

/*
 * Function to invoke function in the out.c.
 */
char* callAdd(char* out, char* variableName1, char* variableName2, int row_count, int column_count) {
    if(row_count == 0 && column_count == 0) {
        snprintf(out, 2048, "addS(%s, %s)", variableName1, variableName2);
    } else {
        snprintf(out, 2048, "add(%s, %s, %d, %d)", variableName1, variableName2, row_count, column_count);
    }
    return out;
}

/*
 * Function to invoke function in the out.c.
 */
char* callSubtract(char* out, char* variableName1, char* variableName2, int row_count, int column_count) {
    if(row_count == 0 && column_count == 0) {
        snprintf(out, 2048, "subtractS(%s, %s)", variableName1, variableName2);
    } else {
        snprintf(out, 2048, "subtract(%s, %s, %d, %d)", variableName1, variableName2, row_count, column_count);
    }
    return out;
}

/*
 * Function to invoke function in the out.c.
 */
char* callMultiply(char* out, char* variableName1, char* variableName2, int row_count1, int column_count1, int row_count2, int column_count2) {
    if(row_count1 == 0 && row_count2 == 0 && column_count1 == 0 && column_count2 == 0) {
        snprintf(out, 2048, "multiplySS(%s, %s)", variableName1, variableName2);
    } else if(row_count1 == 0 && column_count1 == 0) {
        snprintf(out, 2048, "multiplySM(%s, %s, %d, %d)", variableName1, variableName2, row_count2, column_count2);
    } else {
        snprintf(out, 2048, "multiply(%s, %s, %d, %d, %d, %d)",
                 variableName1, variableName2, row_count1, column_count1, row_count2, column_count2);
    }
    return out;
}
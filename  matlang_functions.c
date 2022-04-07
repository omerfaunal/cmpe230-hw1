#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"

extern struct Scalar scalars[];

void error(int line);

char* printSep() {
    return "printf(\"%s\", \"----------\");\n";
}

char *declarePrintId(char *out) {
    snprintf(out, 2048, "void *printId(float **variable, int row_count, int column_count){\n"
           "for(int i = 0; i < row_count; i++){\n"
           "    for(int j = 0; j < column_count; j++) {\n"
           "        if(fabs(variable[i][j] - round(variable[i][j])) < 0.000001) {\n"
           "        printf(\"%s\\n\",(int) round(variable[i][j]));\n"
           "       } \n"
           "       else {\n"
           "        printf(\"%s\\n\",(float) variable[i][j]);\n"
           "       }"
           "    }\n"
           "}\n"
           "}\n", "%%d", "%%f");
    return out;
}

char *declarePrintIdS(char *out) {
    snprintf(out, 2048, "void *printIdS(float variable){\n"
                        "    if(fabs(variable - round(variable)) < 0.000001) {\n"
                        "        printf(\"%s\\n\",(int) round(variable));\n"
                        "    } \n"
                        "    else {\n"
                        "        printf(\"%s\\n\",(float) variable);\n"
                        "    }"
                        "}\n", "%%d", "%%f");
    return out;
}

char* callPrintId(char* out, char* variableName, int row_count, int column_count) {
    if(row_count == 0 && column_count == 0) {
        snprintf(out, 2048, "printIdS(%s)", variableName);
    } else {
        snprintf(out, 2048, "printId(%s, %d, %d);\n", variableName, row_count, column_count);
    }
    return out;
}

////Careful! This functions only return the sqrt value and changes the scalar value in this program.
////After using calculateSqrt function, make sure that you also use scalarValueAssignment function.
//char* calculateSqrt(char* out, int lineNo, char* variableName, char* sqrtName) {
//    float value;
//    for(int i = 0; i < 2048; i++){
//        struct Scalar currentScalar = scalars[i];
//        if(strcmp(currentScalar.name, variableName) == 0){
//            value = currentScalar.value;
//            if(value < 0){
//                error(lineNo);
//                return -1;
//            }
//            currentScalar.value = sqrtf(value);
//            scalars[i] = currentScalar;
//            snprintf(out, 2048, "sqrt(%s)", variableName);
//            return out;
//        }
//    }
//    error(lineNo);
//    return -1;
//}

char* singleForLoop(char* out,char* id, char* expr1, char* expr2, char* expr3) {
    snprintf(out, 2048, "for(float %s = %s; %s < %s; %s += %s) {\n",id, expr1,id, expr2,id, expr3);
    return out;
}

char* doubleForLoop(char* out,char* id1, char* id2, char* expr1, char* expr2, char* expr3, char* expr4, char* expr5, char* expr6) {
    snprintf(out, 2048, "for(float %s = %s; %s < %s; %s += %s ) { \n for(float %s = %s; %s < %s; %s += %s )\n",
             id1, expr1, id1, expr2, id1, expr3, id2, expr4, id2, expr5, id2, expr6);
    return out;
}

char* declareSqrtFunction(char* out) {
    snprintf(out, 2048, "float sqrt_(float value) {\n"
                        "   return sqrtf(value);\n"
                        "}\n");
    return out;
}

char* declareChooseFunction(char* out) {
    snprintf(out, 2048, "float choose(float expr1, float expr2, float expr3, float expr4) {\n"
                        "    float out;\n"
                        "    if(expr1 == 0) {\n"
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

char *declareAddFunctionS(char *out) {
    snprintf(out, 2048, "float addS(float val1, float val2) {\n"
                        "    return val1 + val2;\n"
                        "}\n");
    return out;
}

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

char *declareSubtractFunctionS(char *out) {
    snprintf(out, 2048, "float subtractS(float val1, float val2) {\n"
                        "   return val1 - val2;\n"
                        "}\n");
}

char* declareMultiplyFunction(char* out) {
    snprintf(out, 2048, "float** multiply(float** arr1, float** arr2, int row_count1, int column_count1, int row_count2, int column_count2) {\n"
                        "    float** outArr2 = (float**) calloc(row_count1, sizeof(float*));\n"
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

char *declareMultiplyFunctionSS(char *out) {
    snprintf(out, 2048, "float multiplySS(float val1, float val2) {\n"
                        "   return val1 * val2;\n"
                        "}\n");
}

char *declareMultiplyFuctionSM(char *out) {
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
}

char* callSqrt(char* out, char* variableName) {
    snprintf(out, 2048, "sqrt(%s)\n", variableName);
    return out;
}

char* callChoose(char* out, char* expr1, char* expr2, char* expr3, char* expr4) {
    snprintf(out, 2048, "choose(%s, %s, %s, %s)\n", expr1, expr2, expr3, expr4);
    return out;
}

char* callTranspose(char* out, char* variableName, int row_count, int column_count) {
    if(row_count == 0 && column_count == 0) {
        snprintf(out, 2048, variableName);
    } else {
        snprintf(out, 2048, "transpose(%s, %d, %d)\n", variableName, row_count, column_count);
    }
    return out;
}

char* callAdd(char* out, char* variableName1, char* variableName2, int row_count, int column_count) {
    if(row_count == 0 && column_count == 0) {
        snprintf(out, 2048, "addS(%s, %s)", variableName1, variableName2);
    } else {
        snprintf(out, 2048, "add(%s, %s, %d, %d)", variableName1, variableName2, row_count, column_count);
    }
    return out;
}

char* callSubtract(char* out, char* variableName1, char* variableName2, int row_count, int column_count) {
    if(row_count == 0 && column_count == 0) {
        snprintf(out, 2048, "subtractS(%s, %s)", variableName1, variableName2);
    } else {
        snprintf(out, 2048, "subtract(%s, %s, %d, %d)", variableName1, variableName2, row_count, column_count);
    }
    return out;
}

char* callMultiply(char* out, char* variableName1, char* variableName2, int row_count1, int column_count1, int row_count2, int column_count2) {
    if(row_count1 == 0 && row_count2 == 0 && column_count1 == 0 && column_count2 == 0) {
        snprintf(out, 2048, "multiplySS(%s, %s)", variableName1, variableName2);
    } else if(row_count1 == 0 && row_count2 == 0) {
        snprintf(out, 2048, "multiplySM(%s, %s, %d, %d)", variableName1, variableName2, row_count2, column_count2);
    } else {
        snprintf(out, 2048, "multiply(%s, %s, %d, %d, %d, %d)",
                 variableName1, variableName2, row_count1, column_count1, row_count2, column_count2);
    }
    return out;
}
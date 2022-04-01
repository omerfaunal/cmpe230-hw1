#include <math.h>
#include <stdio.h>
#include <string.h>
#include "structs.h"

extern struct Scalar scalars[];

void error(int line);

char* printSep() {
    return "printf(\"%s\", \"----------\");\n";
}

char* printId(char* out, char* variableName) {
    snprintf(out, 512, "printf(\"%s\", %s);\n","%s", variableName);
    return out;
}

////Careful! This functions only return the sqrt value and changes the scalar value in this program.
////After using calculateSqrt function, make sure that you also use scalarValueAssignment function.
//char* calculateSqrt(char* out, int lineNo, char* variableName, char* sqrtName) {
//    float value;
//    for(int i = 0; i < 256; i++){
//        struct Scalar currentScalar = scalars[i];
//        if(strcmp(currentScalar.name, variableName) == 0){
//            value = currentScalar.value;
//            if(value < 0){
//                error(lineNo);
//                return -1;
//            }
//            currentScalar.value = sqrtf(value);
//            scalars[i] = currentScalar;
//            snprintf(out, 512, "sqrt(%s)", variableName);
//            return out;
//        }
//    }
//    error(lineNo);
//    return -1;
//}

char* singleForLoop(char* out, char* expr1, char* expr2, char* expr3) {
    snprintf(out, 512, "for(int i = %s; i < %s; i += %s) {\n", expr1, expr2, expr3);
    return out;
}

char* doubleForLoop(char* out, char* expr1, char* expr2, char* expr3, char* expr4, char* expr5, char* expr6) {
    snprintf(out, 512, "for(int i = %s; i < %s; i += %s ) { \n for(int j = %s; j < %s; j += %s )\n", expr1, expr2, expr3, expr4, expr5, expr6);
    return out;
}

char* declareChooseFunction(char* out) {
    snprintf(out, 512, "char* choose(char* expr1, char* expr2, char* expr3, char* expr4) {\n"
                       "    if(expr1 == 0) {\n"
                       "        return expr2;\n"
                       "    } else if (expr1 > 0) {\n"
                       "        return expr3;\n"
                       "    } else {\n"
                       "        return expr4;\n"
                       "    }\n"
                       "}\n");
    return out;
}

char* declareTransposeFunction(char* out) {
    snprintf(out, 512, "int** transpose(int** arr, int row_count, int column_count) {\n"
                       "    if(row_count == 0) {\n"
                       "        return arr;\n"
                       "    }\n"
                       "   int** outArr = (int**) calloc(column_count, sizeof(int*));\n"
                       "   for(int i = 0; i < column_count; i++) {\n"
                       "       outArr[i] = (int*) calloc(row_count, sizeof(int));\n"
                       "       for(int j = 0; j < row_count; j++) {\n"
                       "           outArr[i][j] = arr[j][i];\n"
                       "       }\n"
                       "   }    return outArr;\n"
                       "}\n");
    return out;
}

char* callChoose(char* out, char* expr1, char* expr2, char* expr3, char* expr4) {
    snprintf(out, 512, "choose(%s, %s, %s, %s);\n", expr1, expr2, expr3, expr4);
    return out;
}

char* declareSqrtFunction(char* out) {
    snprintf(out, 512, "float sqrt(float value) {return sqrtf(value);}\n");
    return out;
}

char* callSqrt(char* out, char* variableName) {
    snprintf(out, 512, "sqrt(float %s);\n", variableName);
    return out;
}

char* declareAddFunction(char* out) {
    snprintf(out, 1024, "int** add(int** arr1, int** arr2, int row_count, int column_count) {\n"
                        "    if(row_count == 0) {\n"
                        "        int** outArr = (int**) calloc(1, sizeof(int*));\n"
                        "        outArr[0] = (int*) calloc(1, sizeof(int));\n"
                        "        outArr[0][0] = arr1[0][0] + arr2[0][0];\n"
                        "        return outArr;\n"
                        "    }\n"
                        "    int** outArr = (int**) calloc(row_count, sizeof(int*));\n"
                        "    for(int i = 0; i < row_count; i++) {\n"
                        "        outArr[i] = (int*) calloc(column_count, sizeof(int));\n"
                        "        for(int j = 0; j < column_count; j++){\n"
                        "            outArr[i][j] = arr1[i][j] + arr2[i][j];\n"
                        "        }\n"
                        "    }\n"
                        "    return outArr;\n"
                        "}\n");
    return out;
}

char* declareSubtractFunction(char* out) {
    snprintf(out, 1024, "int** subtract(int** arr1, int** arr2, int row_count, int column_count) {\n"
                        "    if(row_count == 0) {\n"
                        "        int** outArr = (int**) calloc(1, sizeof(int*));\n"
                        "        outArr[0] = (int*) calloc(1, sizeof(int));\n"
                        "        outArr[0][0] = arr1[0][0] - arr2[0][0];\n"
                        "        return outArr;\n"
                        "    }\n"
                        "    int** outArr = (int**) calloc(row_count, sizeof(int*));\n"
                        "    for(int i = 0; i < row_count; i++) {\n"
                        "        outArr[i] = (int*) calloc(column_count, sizeof(int));\n"
                        "        for(int j = 0; j < column_count; j++){\n"
                        "            outArr[i][j] = arr1[i][j] - arr2[i][j];\n"
                        "        }\n"
                        "    }\n"
                        "    return outArr;\n"
                        "}\n");
    return out;
}


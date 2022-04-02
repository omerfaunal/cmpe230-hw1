#include <math.h>
#include <stdio.h>
#include <string.h>
#include "structs.h"

extern struct Scalar scalars[];

void error(int line);

char* printSep() {
    return "printf(\"%s\", \"----------\");\n";
}

char *declarePrintId(char *out) {
    snprintf(out, 2048, "void *printId(int **variable, int row_count, int column_count){\n"
                       "int m = row_count;\n"
                       "if(m == 0) {\n"
                       "    m += 1;\n"
                       "}\n"
                       "for(int i = 0; i < m; i++){\n"
                       "    printf(variable[i]);\n"
                       "}\n"
                       "}\n");
    return out;
}

char* callPrintId(char* out, char* variableName, int row_count, int column_count) {
    snprintf(out, 2048, "printId(%s, %d, %d);\n", variableName, row_count, column_count);
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
    snprintf(out, 2048, "for(int %s = %s; %s < %s; %s += %s) {\n",id, expr1,id, expr2,id, expr3);
    return out;
}

char* doubleForLoop(char* out,char* id1, char* id2, char* expr1, char* expr2, char* expr3, char* expr4, char* expr5, char* expr6) {
    snprintf(out, 2048, "for(int %s = %s; %s < %s; %s += %s ) { \n for(int %s = %s; %s < %s; %s += %s )\n",id1, expr1, id1, expr2, id1, expr3, id2, expr4, id2, expr5, id2, expr6);
    return out;
}

char* declareSqrtFunction(char* out) {
    snprintf(out, 2048, "float sqrt_(float value) {return sqrtf(value);}\n");
    return out;
}

char* declareChooseFunction(char* out) {
    snprintf(out, 2048, "char* choose(char* expr1, char* expr2, char* expr3, char* expr4) {\n"
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
    snprintf(out, 2048, "int** transpose(int** arr, int row_count, int column_count) {\n"
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

char* declareAddFunction(char* out) {
    snprintf(out, 2048, "int** add(int** arr1, int** arr2, int row_count, int column_count) {\n"
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
    snprintf(out, 2048, "int** subtract(int** arr1, int** arr2, int row_count, int column_count) {\n"
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

char* declareMultiplyFunction(char* out) {
    snprintf(out, 2048, "int** multiply(int** arr1, int** arr2, int row_count1, int column_count1, int row_count2, int column_count2) {\n"
                        "    if(row_count1 == 0 && row_count2 == 0) {\n"
                        "        int** outArr = (int**) calloc(1, sizeof(int*));\n"
                        "        outArr[0] = (int*) calloc(1, sizeof(int));\n"
                        "        outArr[0][0] = arr1[0][0] * arr2[0][0];\n"
                        "        return outArr;\n"
                        "    }\n"
                        "    int** outArr = (int**) calloc(row_count2, sizeof(int*));\n"
                        "    if(row_count1 == 0) {\n"
                        "        for(int i = 0; i < row_count2; i++) {\n"
                        "            outArr[i] = (int*) calloc(column_count2, sizeof(int));\n"
                        "            for(int j = 0; j < column_count2; j++){\n"
                        "                outArr[i][j] = arr2[i][j] * arr1[0][0];\n"
                        "            }\n"
                        "        }\n"
                        "        return outArr;\n"
                        "    }\n"
                        "\n"
                        "    int** outArr2 = (int**) calloc(row_count1, sizeof(int*));\n"
                        "    if(row_count2 == 0) {\n"
                        "        for(int i = 0; i < row_count1; i++) {\n"
                        "            outArr2[i] = (int*) calloc(column_count1, sizeof(int));\n"
                        "            for(int j = 0; j < column_count1; j++){\n"
                        "                outArr2[i][j] = arr1[i][j] * arr2[0][0];\n"
                        "            }\n"
                        "        }\n"
                        "        return outArr2;\n"
                        "    }\n"
                        "    else {\n"
                        "        int** outArr3 = (int**) calloc(row_count1, sizeof(int*));\n"
                        "        for (int i = 0; i < row_count1; ++i) {\n"
                        "            outArr3[i] = (int*) calloc(column_count2, sizeof(int));\n"
                        "            for (int j = 0; j < column_count2; ++j) {\n"
                        "                for (int k = 0; k < column_count1; ++k) {\n"
                        "                    outArr3[i][j] += arr1[i][k] * arr2[k][j];\n"
                        "                }\n"
                        "            }\n"
                        "        }\n"
                        "        return outArr3;\n"
                        "    }\n"
                        "}\n");
    return out;
}

char* callSqrt(char* out, char* variableName) {
    snprintf(out, 2048, "sqrt(float %s);\n", variableName);
    return out;
}

char* callChoose(char* out, char* expr1, char* expr2, char* expr3, char* expr4) {
    snprintf(out, 2048, "choose(%s, %s, %s, %s);\n", expr1, expr2, expr3, expr4);
    return out;
}

char* callTranspose(char* out, char* variableName, int row_count, int column_count) {
    snprintf(out, 2048, "transpose(%s, %d, %d);\n", variableName, row_count, column_count);
    return out;
}

char* callAdd(char* out, char* variableName1, char* variableName2, int row_count, int column_count) {
    snprintf(out, 2048, "add(%s, %s, %d, %d);", variableName1, variableName2, row_count, column_count);
    return out;
}

char* callSubtract(char* out, char* variableName1, char* variableName2, int row_count, int column_count) {
    snprintf(out, 2048, "subtract(%s, %s, %d, %d);", variableName1, variableName2, row_count, column_count);
    return out;
}

char* callMultiply(char* out, char* variableName1, char* variableName2, int row_count1, int column_count1, int row_count2, int column_count2) {
    snprintf(out, 2048, "multiply(%s, %s, %d, %d, %d, %d);", variableName1, variableName2, row_count1, column_count1, row_count2, column_count2);
    return out;
}
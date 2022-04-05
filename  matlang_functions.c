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
           "if(row_count == 0) {\n"
           "    row_count++;\n"
           "    column_count++;\n"
           "}\n"
           "for(int i = 0; i < row_count; i++){\n"
           "    for(int j = 0; j < column_count; j++) {\n"
           "        if(abs(variable[i][j] - round(variable[i][j]) < 0.000001)) {\n"
           "        printf(\"%s\\n\",(int) variable[i][j]);\n"
           "       } \n"
           "       else {\n"
           "        printf(\"%s\\n\",(float) variable[i][j]);\n"
           "       }"
           "    }\n"
           "}\n"
           "}\n", "%d", "%f.");
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
    if(expr1[0] == '{') {
        int i = 3;
        while(expr1[i] != '}') {
            i++;
        }
        // expr1[i] == '}'
        memcpy(expr1, &expr1[2], i - 2);
        expr1[i - 2] = '\0';
    } else {
        char *new_expr = (char*) calloc(2048, sizeof(char));
        strcpy(new_expr, expr1);
        strcat(new_expr, "[0][0]");
        expr1 = strdup(new_expr);
        free(new_expr);
    }

    if(expr2[0] == '{') {
        int i = 3;
        while(expr2[i] != '}') {
            i++;
        }
        // expr2[i] == '}'
        memcpy(expr2, &expr2[2], i - 2);
        expr2[i - 2] = '\0';
    } else {
        char *new_expr = (char*) calloc(2048, sizeof(char));
        strcpy(new_expr, expr2);
        strcat(new_expr, "[0][0]");
        expr2 = strdup(new_expr);
        free(new_expr);
    }

    if(expr3[0] == '{') {
        int i = 3;
        while(expr3[i] != '}') {
            i++;
        }
        // expr3[i] == '}'
        memcpy(expr3, &expr3[2], i - 2);
        expr3[i - 2] = '\0';
    } else {
        char *new_expr = (char*) calloc(2048, sizeof(char));
        strcpy(new_expr, expr3);
        strcat(new_expr, "[0][0]");
        expr3 = strdup(new_expr);
        free(new_expr);
    }
    snprintf(out, 2048, "for(int %s = %s; %s < %s; %s += %s) {\n",id, expr1,id, expr2,id, expr3);
    return out;
}

char* doubleForLoop(char* out,char* id1, char* id2, char* expr1, char* expr2, char* expr3, char* expr4, char* expr5, char* expr6) {
    if(expr1[0] == '{') {
        int i = 3;
        while(expr1[i] != '}') {
            i++;
        }
        // expr1[i] == '}'
        memcpy(expr1, &expr1[2], i - 2);
        expr1[i - 2] = '\0';
    } else {
        char *new_expr = (char*) calloc(2048, sizeof(char));
        strcpy(new_expr, expr1);
        strcat(new_expr, "[0][0]");
        expr1 = strdup(new_expr);
        free(new_expr);
    }

    if(expr2[0] == '{') {
        int i = 3;
        while(expr2[i] != '}') {
            i++;
        }
        // expr2[i] == '}'
        memcpy(expr2, &expr2[2], i - 2);
        expr2[i - 2] = '\0';
    } else {
        char *new_expr = (char*) calloc(2048, sizeof(char));
        strcpy(new_expr, expr2);
        strcat(new_expr, "[0][0]");
        expr2 = strdup(new_expr);
        free(new_expr);
    }

    if(expr3[0] == '{') {
        int i = 3;
        while(expr3[i] != '}') {
            i++;
        }
        // expr3[i] == '}'
        memcpy(expr3, &expr3[2], i - 2);
        expr3[i - 2] = '\0';
    } else {
        char *new_expr = (char*) calloc(2048, sizeof(char));
        strcpy(new_expr, expr3);
        strcat(new_expr, "[0][0]");
        expr3 = strdup(new_expr);
        free(new_expr);
    }

    if(expr4[0] == '{') {
        int i = 3;
        while(expr4[i] != '}') {
            i++;
        }
        // expr4[i] == '}'
        memcpy(expr4, &expr4[2], i - 2);
        expr4[i - 2] = '\0';
    } else {
        char *new_expr = (char*) calloc(2048, sizeof(char));
        strcpy(new_expr, expr4);
        strcat(new_expr, "[0][0]");
        expr4 = strdup(new_expr);
        free(new_expr);
    }

    if(expr5[0] == '{') {
        int i = 3;
        while(expr5[i] != '}') {
            i++;
        }
        // expr5[i] == '}'
        memcpy(expr5, &expr5[2], i - 2);
        expr5[i - 2] = '\0';
    } else {
        char *new_expr = (char*) calloc(2048, sizeof(char));
        strcpy(new_expr, expr5);
        strcat(new_expr, "[0][0]");
        expr5 = strdup(new_expr);
        free(new_expr);
    }

    if(expr6[0] == '{') {
        int i = 3;
        while(expr6[i] != '}') {
            i++;
        }
        // expr6[i] == '}'
        memcpy(expr6, &expr6[2], i - 2);
        expr6[i - 2] = '\0';
    } else {
        char *new_expr = (char*) calloc(2048, sizeof(char));
        strcpy(new_expr, expr6);
        strcat(new_expr, "[0][0]");
        expr6 = strdup(new_expr);
        free(new_expr);
    }
    snprintf(out, 2048, "for(int %s = %s; %s < %s; %s += %s ) { \n for(int %s = %s; %s < %s; %s += %s )\n",id1, expr1, id1, expr2, id1, expr3, id2, expr4, id2, expr5, id2, expr6);
    return out;
}

char* declareSqrtFunction(char* out) {
    snprintf(out, 2048, "float **sqrt_(float value) {\n"
                        "float **out = (float**) calloc(1, sizeof(float*));\n"
                        "out[0] = calloc(1, sizeof(float));\n"
                        "out[0][0] = sqrtf(value);\n"
                        "return out;\n"
                        "}\n");
    return out;
}

char* declareChooseFunction(char* out) {
    snprintf(out, 2048, "float **choose(float expr1, float expr2, float expr3, float expr4) {\n"
                        "float **out = (float**) calloc(1, sizeof(float*));\n"
                        "out[0] = calloc(1, sizeof(float));\n"
                       "    if(expr1 == 0) {\n"
                       "        out[0][0] = expr2;\n"
                       "    } else if (expr1 > 0) {\n"
                       "        out[0][0] = expr3;\n"
                       "    } else {\n"
                       "        out[0][0] = expr4;\n"
                       "    }\n"
                       "return out;\n"
                       "}\n");
    return out;
}

char* declareTransposeFunction(char* out) {
    snprintf(out, 2048, "float** transpose(float** arr, int row_count, int column_count) {\n"
                       "    if(row_count == 0) {\n"
                       "        return arr;\n"
                       "    }\n"
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
                        "    if(row_count == 0) {\n"
                        "        float** outArr = (float**) calloc(1, sizeof(float*));\n"
                        "        outArr[0] = (float*) calloc(1, sizeof(float));\n"
                        "        outArr[0][0] = arr1[0][0] + arr2[0][0];\n"
                        "        return outArr;\n"
                        "    }\n"
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

char* declareMultiplyFunction(char* out) {
    snprintf(out, 2048, "float** multiply(float** arr1, float** arr2, int row_count1, int column_count1, int row_count2, int column_count2) {\n"
                        "    if(row_count1 == 0 && row_count2 == 0) {\n"
                        "        float** outArr = (float**) calloc(1, sizeof(float*));\n"
                        "        outArr[0] = (float*) calloc(1, sizeof(float));\n"
                        "        outArr[0][0] = arr1[0][0] * arr2[0][0];\n"
                        "        return outArr;\n"
                        "    }\n"
                        "    float** outArr = (float**) calloc(row_count2, sizeof(float*));\n"
                        "    if(row_count1 == 0) {\n"
                        "        for(int i = 0; i < row_count2; i++) {\n"
                        "            outArr[i] = (float*) calloc(column_count2, sizeof(float));\n"
                        "            for(int j = 0; j < column_count2; j++){\n"
                        "                outArr[i][j] = arr2[i][j] * arr1[0][0];\n"
                        "            }\n"
                        "        }\n"
                        "        return outArr;\n"
                        "    }\n"
                        "\n"
                        "    float** outArr2 = (float**) calloc(row_count1, sizeof(float*));\n"
                        "    if(row_count2 == 0) {\n"
                        "        for(int i = 0; i < row_count1; i++) {\n"
                        "            outArr2[i] = (float*) calloc(column_count1, sizeof(float));\n"
                        "            for(int j = 0; j < column_count1; j++){\n"
                        "                outArr2[i][j] = arr1[i][j] * arr2[0][0];\n"
                        "            }\n"
                        "        }\n"
                        "        return outArr2;\n"
                        "    }\n"
                        "    else {\n"
                        "        float** outArr3 = (float**) calloc(row_count1, sizeof(float*));\n"
                        "        for (int i = 0; i < row_count1; ++i) {\n"
                        "            outArr3[i] = (float*) calloc(column_count2, sizeof(float));\n"
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
    snprintf(out, 2048, "sqrt(%s[0][0]);\n", variableName);
    return out;
}

char* callChoose(char* out, char* expr1, char* expr2, char* expr3, char* expr4) {
    snprintf(out, 2048, "choose(%s[0][0], %s[0][0], %s[0][0], %s[0][0]);\n", expr1, expr2, expr3, expr4);
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
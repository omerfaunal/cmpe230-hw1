#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#define MAX_CHAR 256
#define MAX_LINE 256

struct Matrix {  // Note that vectors are considered n x 1 matrices.
    char *name;
    int row_count;
    int column_count;
    int *begin;  // This points to the position (1, 1) of the matrix.
};

struct Scalar {
    char *name;
    float value;
};

const char *terminals[22] = {"scalar", "vector", "matrix", "[", "]", ",", "{", "}",
                             "*", "+", "-", "tr", "(", ")", "sqrt", "choose",
                             "#", "for", "in", ":", "printsep", "print"};
                            // Note that numeric characters and variable names aren't included here.

struct Matrix matrices[MAX_LINE];  // Matrix variables will be stored here.
struct Matrix* matrixListPointer = matrices; //This is a pointer for traversing matrices list.
struct Scalar scalars[MAX_LINE];  // Scalar variables will be stored here.
struct Scalar* scalarListPointer = scalars;//This is a pointer for traversing scalars list.


void error(int line);
char* scalarValueDeclaration(char* out, char* variableName, float value);

int main(int argc, char *argv[]) {

    FILE *fp;
    char line[MAX_CHAR];

    if(argc != 2) {
        printf("Give filename as as command line argument.\n");
        return 1;
    }

    fp = fopen(argv[1], "r");
    if(fp == NULL) {
        printf("Cannot open %s\n", argv[1]);
        return 1;
    }

    FILE *out_file = fopen("C:\\Users\\Eren\\CLionProjects\\cmpe230-hw1\\out.c", "w");
    if(out_file == NULL) {
        printf("Cannot open out.c\n");
        return 1;
    }

    int line_number = 1;
    while(fgets(line, MAX_CHAR, fp) != NULL) {
        char *pcline = &line[0];
        char sepline[MAX_CHAR * 2] = {'\0'};
        char *pcsepline = &sepline[0];
        char token[MAX_CHAR] = {'\0'};
        char *pctoken = &token[0];
        int is_float = 0;
        int scalar_declaration = 0;
        int matrix_declaration = 0;
        while(*pcline != '\0') {

            if(*pcline == '\n' || *pcline == '#') {
                if(pctoken != &token[0]) {
                    error(line_number);
                } else {
                    *pcsepline = '\n';
                    break;
                }
            }

            if(isspace(*pcline)) {
                if(pctoken != &token[0]) {
                    error(line_number);
                } else {
                    pcline++;
                    continue;
                }
            }


            *pcsepline = *pcline;
            pcsepline++;
            *pctoken = *pcline;
            pctoken++;

            // New variable name detection
            if((scalar_declaration == 1 || matrix_declaration == 1) &&
            !(isalpha(*(pcline + 1)) || isdigit(*(pcline + 1)) || *(pcline + 1) == '_')) {
                if(isdigit(token[0])) {
                    error(line_number);
                }

                for(int i = 0; i < 22; i++) {
                    if(strcmp(token, terminals[i]) == 0) {
                        error(line_number);
                    }
                }

                *pcsepline = ' ';
                pcsepline++;

                while(pctoken != &token[0]) {
                    pctoken--;
                    *pctoken = '\0';
                }

                scalar_declaration = 0;
                matrix_declaration = 0;
                pcline++;
                continue;
            }


            // Keyword detection
            int matched = 0;
            for(int i = 0; i < 22; i++) {
                if(strcmp(token, terminals[i]) == 0 &&
                (pctoken - &token[0] == 1 ||
                !(isalpha(*(pcline + 1)) || isdigit(*(pcline + 1)) || *(pcline + 1) == '_'))) {
                    matched = 1;

                    if(i == 0) {
                        scalar_declaration = 1;
                    } else if(i <= 2) {
                        matrix_declaration = 1;
                    }

                    *pcsepline = ' ';
                    pcsepline++;
                    while(pctoken != &token[0]) {
                        pctoken--;
                        *pctoken = '\0';
                    }
                }
                if(matched) {
                    break;
                }
            }
            if(matched) {
                pcline++;
                continue;
            }

            // Number detection
            if(isdigit(token[0]) && !isdigit(*(pcline + 1))) {
                if(*(pcline + 1) == '.') {
                    if(!is_float) {
                        is_float = 1;
                        pcline++;
                        continue;
                    } else {
                        error(line_number);
                    }
                }
                *pcsepline = ' ';
                pcsepline++;
                while(pctoken != &token[0]) {
                    pctoken--;
                    *pctoken = '\0';
                }
                pcline++;
                continue;
            }

            // Variable detection
            if(!(isalpha(*(pcline + 1)) || isdigit(*(pcline + 1)) || *(pcline + 1) == '_')) {
                for (int i = 0; i < MAX_LINE; i++) {
                    if (strcmp(token, matrices[i].name) == 0 || strcmp(token, scalars[i].name) == 0) {
                        *pcsepline = ' ';
                        pcsepline++;
                        while(pctoken != &token[0]) {
                            pctoken--;
                            *pctoken = '\0';
                        }
                        break;
                    }
                }
            }

            pcline++;
        }

        if(pctoken != &token[0]) {
            error(line_number);
        }

        char **final_line = (char**) calloc(MAX_CHAR, sizeof(char*));
        short int token_ctr = 0;
        final_line[token_ctr] = strtok(sepline, " ");
        token_ctr++;
        char *final_token;
        while((final_token = strtok(NULL, " ")) != NULL) {
            final_line[token_ctr] = final_token;
            token_ctr++;
        }

        final_line = (char**) realloc(final_line, token_ctr * sizeof(char*));

//        for(int i = 0; i < token_ctr; i++) {
//            printf(final_line[i]);
//            printf(" ");
//        }
//        printf("\n");

        line_number++;
    }

    fclose(fp);
    fclose(out_file);
    return 0;
}

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


void error(int line) {
    //TODO
    printf("Error (Line %d)\n", line);
}
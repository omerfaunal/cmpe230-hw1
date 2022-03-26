#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "structs.h"

#define MAX_CHAR 256
#define MAX_LINE 256

const char *terminals[22] = {"scalar", "vector", "matrix", "[", "]", ",", "{", "}",
                             "*", "+", "-", "tr", "(", ")", "sqrt", "choose",
                             "#", "for", "in", ":", "printsep", "print"};
                            // Note that numeric characters and variable names aren't included here.

struct Matrix matrices[MAX_LINE];  // Matrix variables will be stored here.
struct Matrix* matrixListPointer = matrices; //This is a pointer for traversing matrices list.
short int matrix_count = 0;
struct Scalar scalars[MAX_LINE];  // Scalar variables will be stored here.
struct Scalar* scalarListPointer = scalars;//This is a pointer for traversing scalars list.
short int scalar_count = 0;

void error(int line);
char* scalarValueDeclaration(char* out, char* variableName, float value);
char* eval(char** line, short int size);

int line_number = 1;

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

    while(fgets(line, MAX_CHAR, fp) != NULL) {
        char *pcline = &line[0];
        char sepline[MAX_CHAR * 2] = {'\0'};
        char *pcsepline = &sepline[0];
        char token[MAX_CHAR] = {'\0'};
        char *pctoken = &token[0];
        int is_float = 0;
        int new_declaration = 0;
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
            if(new_declaration == 1 &&
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

                new_declaration = 0;
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
                    new_declaration = 1;

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
        eval(final_line, token_ctr);  // TODO: Write eval's return value to out.c
//        printf("%d\n", token_ctr);

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


void error(int line) {
    //TODO
    printf("Error (Line %d)\n", line);
    exit(EXIT_FAILURE);
}
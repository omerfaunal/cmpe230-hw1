#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_CHAR 256
#define MAX_LINE 256

struct Matrix {  // Note that scalars are considered 1 x 1 matrices and vectors are considered n x 1 matrices.
    int row_count;
    int column_count;
    int *begin;  // This points to the position (1, 1) of the matrix.
};

const char *terminals[22] = {"scalar", "vector", "matrix", "[", "]", ",", "{", "}",
                             "*", "+", "-", "tr", "(", ")", "sqrt", "choose",
                             "#", "for", "in", ":", "printsep", "print"};
                            // Note that numeric characters and variable names aren't included here.

struct Matrix variables[MAX_LINE];  // Variables will be stored here.

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

    while(fgets(line, MAX_CHAR, fp) != NULL) {
        char *pcline = &line[0];
        char sepline[MAX_CHAR * 2];
        char *pcsepline = &sepline[0];
        char token[MAX_CHAR];
        char *pctoken = &token[0];
        while(*pcline != '\0') {
            *pcsepline = *pcline;
            pcsepline++;
            *pctoken = *pcline;
            pctoken++;

            // Keyword detection
            int matched = 0;
            for(int i = 0; i < 22; i++) {
                if(strcmp(token, terminals[i]) == 0 && !isalpha(*(pcline + 1))) {
                    matched = 1;
                    *pcsepline = ' ';
                    pcsepline++;
                    while(pctoken != &token[0]) {
                        *pctoken = '\0';
                        pctoken--;
                    }
                }
                if(matched) {
                    break;
                }
            }
            if(matched) {
                continue;
            }

            // Integer detection
            if(isdigit(token[0]) && !isdigit(*(pcline + 1))) {
                *pcsepline = ' ';
                pcsepline++;
                while(pctoken != &token[0]) {
                    *pctoken = '\0';
                    pctoken--;
                }
            }

            //TODO: Check if token is a variable name.

            pcline++;
        }
    }

    fclose(fp);
    return 0;
}

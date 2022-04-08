#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "structs.h"

void error(int line);
char* eval(char** line, int size);
char* declareSqrtFunction(char* out);
char* declareChooseFunction(char* out);
char* declareTransposeFunction(char* out);
char* declareAddFunction(char* out);
char* declareSubtractFunction(char* out);
char* declareMultiplyFunction(char* out);
char *declarePrintId(char *out);
char *declarePrintIdS(char *out);
char *declareAddFunctionS(char *out);
char *declareSubtractFunctionS(char *out);
char *declareMultiplyFunctionSS(char *out);
char *declareMultiplyFunctionSM(char *out);

extern const char *terminals[];
extern char out_file_name[];
extern FILE *out_file;
extern int line_number;
extern int scalar_count;
extern struct Scalar scalars[];
extern int matrix_count;
extern struct Matrix matrices[];

int main(int argc, char *argv[]) {
    /*
     * Read lines from file with file pointer
     */
    FILE *fp;
    char line[2048];

    if(argc != 2) {
        printf("Give filename as as command line argument.\n");
        return 1;
    }

    fp = fopen(argv[1], "r");
    if(fp == NULL) {
        printf("Cannot open %s\n", argv[1]);
        return 1;
    }

    strcpy(out_file_name, argv[1]);
    strcpy(out_file_name + strlen(argv[1]) - 3, "c");

    out_file = fopen(out_file_name, "w");
    /*
     * Raise an error if file cannot be opened.
     */
    if(out_file == NULL) {
        printf("Cannot open out.c\n");
        return 1;
    }

    char *buffer = (char*) calloc(2048, sizeof(char));
    /*
     * Prints out.c headers and declaration functions.
     */
    fprintf(out_file, "#include <stdio.h>\n"
                      "#include <stdlib.h>\n"
                      "#include <math.h>\n");
    fprintf(out_file, "%s", declareAddFunction(buffer));
    fprintf(out_file, "%s", declareAddFunctionS(buffer));
    fprintf(out_file, "%s", declareSubtractFunction(buffer));
    fprintf(out_file, "%s", declareSubtractFunctionS(buffer));
    fprintf(out_file, "%s", declareMultiplyFunction(buffer));
    fprintf(out_file, "%s", declareMultiplyFunctionSS(buffer));
    fprintf(out_file, "%s", declareMultiplyFunctionSM(buffer));
    fprintf(out_file, "%s", declareSqrtFunction(buffer));
    fprintf(out_file, "%s", declareTransposeFunction(buffer));
    fprintf(out_file, "%s", declareChooseFunction(buffer));
    fprintf(out_file, "%s", declarePrintId(buffer));
    fprintf(out_file, "%s", declarePrintIdS(buffer));
    fprintf(out_file, "int main() {\n");

    while(fgets(line, 2048, fp) != NULL) {
        char *pcline = &line[0];
        char sepline[2048 * 2] = {'\0'};
        char *pcsepline = &sepline[0];
        char token[2048] = {'\0'};
        char *pctoken = &token[0];
        int is_float = 0;
        int new_declaration = 0;
        int for_loop = 0;
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

            // New variable name / for loop id detection
            if((new_declaration == 1 || for_loop == 1) &&
            !(isalpha(*(pcline + 1)) || isdigit(*(pcline + 1)) || *(pcline + 1) == '_')) {
                if(isdigit(token[0])) {
                    error(line_number);
                }

                for(int i = 0; i < 23; i++) {
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
                if(for_loop == 1 && *(pcline + 1) != ',') {
                    for_loop = 0;
                }
                pcline++;
                continue;
            }


            // Keyword detection
            int matched = 0;
            for(int i = 0; i < 23; i++) {
                if(strcmp(token, terminals[i]) == 0 &&
                (pctoken - &token[0] == 1 ||
                !(isalpha(*(pcline + 1)) || isdigit(*(pcline + 1)) || *(pcline + 1) == '_'))) {
                    matched = 1;
                    if(i <= 2) {
                        new_declaration = 1;
                    } else if(i == 17) {
                        for_loop = 1;
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
                is_float = 0;
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
                for (int i = 0; i < scalar_count; i++) {
                    if (strcmp(token, scalars[i].name) == 0) {
                        *pcsepline = ' ';
                        pcsepline++;
                        while(pctoken != &token[0]) {
                            pctoken--;
                            *pctoken = '\0';
                        }
                        break;
                    }
                }
                if(pctoken != &token[0]) {
                    for (int i = 0; i < matrix_count; i++) {
                        if (strcmp(token, matrices[i].name) == 0) {
                            *pcsepline = ' ';
                            pcsepline++;
                            while (pctoken != &token[0]) {
                                pctoken--;
                                *pctoken = '\0';
                            }
                            break;
                        }
                    }
                }
            }

            pcline++;
        }

        if(pctoken != &token[0]) {
            error(line_number);
        }

        char **final_line = (char**) calloc(2048, sizeof(char*));
        int token_ctr = 0;
        final_line[token_ctr] = strtok(sepline, " ");
        token_ctr++;
        char *final_token;
        while((final_token = strtok(NULL, " ")) != NULL) {
            final_line[token_ctr] = final_token;
            token_ctr++;
        }

        final_line = (char**) realloc(final_line, token_ctr * sizeof(char*));
        fprintf(out_file, eval(final_line, token_ctr));

        line_number++;
    }

    fprintf(out_file, "return 0;\n}\n");

    fclose(fp);
    fclose(out_file);
    return 0;
}
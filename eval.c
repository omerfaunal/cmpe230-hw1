#include <string.h>
#include <stdlib.h>

#define MAX_CHAR 256

void error(int line);
char* scalarValueDeclaration(char* out, char* variableName, float value);
char* matrixDeclaration(char* out, char* variableName, int columnCount, int rowCount);

extern int line_number;

char* eval(char** line, short int size) {  // Note that size also contains the newline character at the end of the line.
    if(size == 1) {
        return "\n";
    }

    // Scalar Declaration
    // TODO: What to do when a previously declared value is declared again?
    if(strcmp(line[0], "scalar") == 0) {
        if(size != 3) {
            error(line_number);
            return "error";
        }
        char out[MAX_CHAR + 50];
        return scalarValueDeclaration(out, line[1], 0);
    }

    // Vector declaration
    // TODO: What to do when a previously declared value is declared again?
    if(strcmp(line[0], "vector") == 0) {
        if(size != 6 || strcmp(line[2], "[") != 0 || strcmp(line[4], "]") != 0) {
            error(line_number);
            return "error";
        }
        if(strchr(line[3], (int) '.') != NULL) {
            error(line_number);
            return "error";
        }
        char out[MAX_CHAR + 50];
        return matrixDeclaration(out, line[1], 1, atoi(line[3]));
    }

    // Matrix declaration
    // TODO: What to do when a previously declared value is declared again?
    if(strcmp(line[0], "matrix") == 0) {
        if(size != 8 || strcmp(line[2], "[") != 0 || strcmp(line[4], ",") != 0 || strcmp(line[6], "]") != 0) {
            error(line_number);
            return "error";
        }
        if(strchr(line[3], (int) '.') != NULL || strchr(line[5], (int) '.') != NULL) {
            error(line_number);
            return "error";
        }
        char out[MAX_CHAR + 50];
        return matrixDeclaration(out, line[1], atoi(line[5]), atoi(line[3]));
    }


}
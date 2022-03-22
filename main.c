#include <stdio.h>

struct Matrix {  // Note that scalars are considered 1 x 1 matrices and vectors are considered n x 1 matrices.
    int row_count;
    int column_count;
    int *begin;  // This points to the position (1, 1) of the matrix.
};

int main(int argc, char *argv[]) {
    FILE *fp;
    char line[256];

    if(argc != 2) {
        printf("Give filename as as command line argument.\n");
        return 1;
    }

    fp = fopen(argv[1], "r");
    if(fp == NULL) {
        printf("Cannot open %s\n", argv[1]);
        return 1;
    }

    while(fgets(line, 256, fp) != NULL) {
        printf("%s", line);
    }

    fclose(fp);
    return 0;
}

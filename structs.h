//
// Created by Eren on 26.03.2022.
//

#ifndef CMPE230_HW1_STRUCTS_H
#define CMPE230_HW1_STRUCTS_H

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

#endif //CMPE230_HW1_STRUCTS_H

//
// Created by peter on 10/5/2022.
//

#ifndef KALMAN_FILTER_MATRIX_H
#define KALMAN_FILTER_MATRIX_H

typedef struct {
    int row;
    int col;
    float **a;
} Matrix;

Matrix *matrix_create(int row, int col, ...);
Matrix *matrix_create_withArr1D(int row, int col, const float *arr);
Matrix *matrix_create_withArr2D(int row, int col, const float **arr);
Matrix *matrix_createSameValue(int row, int col,float value);
Matrix *matrix_createIdentity(int size);
void matrix_setValues(Matrix *m,...);
void matrix_setValuesWithArr1D(Matrix *m,const float *arr);
void matrix_setValuesWithArr2D(Matrix *m,const float **arr);
void matrix_setValuesWithMatrix(Matrix *m,Matrix source);
void matrix_makeDiagonal(Matrix *m);

///////////////////////UTILITIES///////////////////////////
void matrix_print(Matrix m);
Matrix *matrix_copy(Matrix m);

///////////////////////MATRIX OPERATIONS//////////////
Matrix *matrix_transpose(Matrix m);
Matrix *matrix_add(Matrix m1,Matrix m2);
Matrix *matrix_sub(Matrix m1,Matrix m2);
Matrix *matrix_dot(Matrix m1,Matrix m2);
Matrix *matrix_minor(Matrix m,int row,int col);
float matrix_det(Matrix m);
float matrix_cofactor(Matrix m,int row,int col);
Matrix *matrix_cofactorMatrix(Matrix m);
Matrix *matrix_adjoint(Matrix m);
Matrix *matrix_inverse(Matrix m);

///////////////////////SCALAR OPERATIONS/////////////
Matrix *matrix_scale(Matrix m,float scaleFactor);
Matrix *matrix_offset(Matrix m,float offset);

//////////////////////MEMORY DE ALLOCATION/////////
void matrix_destroy(Matrix *m);

///////////////////////MEMORY LEAK CHECK//////////
int matrix_MemoryLeaks();

#endif //KALMAN_FILTER_MATRIX_H

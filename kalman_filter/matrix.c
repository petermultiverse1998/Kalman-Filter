//
// Created by peter on 10/5/2022.
//
#include "matrix.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

long matrixMemoryLeak = 0;
long matrixRowMemoryLeak = 0;
long matrixRowElementMemoryLeak = 0;

//void elementIndex(int rowSize,int i,int j,int *k){
//    *k = (rowSize*i+j);
//}
//
//void elementRowCol(int rowSize,int k,int *i,int*j){
//    *i = k/rowSize;
//    *j = k%rowSize;
//}

Matrix *matrix_allocate(int row, int col) {
    Matrix *matrix = (Matrix *) malloc(sizeof(Matrix));
    matrixMemoryLeak+= sizeof(Matrix);
    matrix->row = row;
    matrix->col = col;

    matrix->a = (float **) malloc(matrix->row * sizeof(float *));
    matrixRowMemoryLeak+= (long )(row*sizeof(float *));
    for (int i = 0; i < matrix->row; ++i, matrixRowElementMemoryLeak+=(long )(col*sizeof(float)))
        matrix->a[i] = (float *) malloc(matrix->col * sizeof(float));
    return matrix;
}

/**
 * Create the matrix and return it's pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param row Row of matrix
 * @param col Col of matrix
 * @param ... elements of type float (a11,a12,a13, a21,a22,a23, a31,a32,a33)
 * @return pointer to created matrix
 */
Matrix *matrix_create(int row, int col, ...) {
    Matrix *matrix = matrix_allocate(row, col);
    //Retrieving element
    va_list lst;
    va_start(lst, col);
    for (int i = 0; i < matrix->row; ++i)
        for (int j = 0; j < matrix->col; ++j)
            matrix->a[i][j] = va_arg(lst, double);
    va_end(lst);

    return matrix;
}

/**
 * Create the matrix and return it's pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param row Row of matrix
 * @param col Col of matrix
 * @param arr elements of type float [a11,a12,a13, a21,a22,a23, a31,a32,a33]
 * @return pointer to created matrix
 */
Matrix *matrix_createWithArr1D(int row, int col, const float *arr) {
    Matrix *matrix = matrix_allocate(row, col);
    //Retrieving element
    for (int i = 0, k = 0; i < matrix->row; ++i)
        for (int j = 0; j < matrix->col; ++j, k++)
            matrix->a[i][j] = arr[k];

    return matrix;
}

/**
 * Create the matrix and return it's pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param row Row of matrix
 * @param col Col of matrix
 * @param arr elements of type float [[a11,a12,a13],[a21,a22,a23],[a31,a32,a33]]
 * @return pointer to created matrix
 */
Matrix *matrix_createWithArr2D(int row, int col, const float **arr) {
    Matrix *matrix = matrix_allocate(row, col);
    //Retrieving element
    for (int i = 0; i < matrix->row; ++i)
        for (int j = 0; j < matrix->col; ++j)
            matrix->a[i][j] = arr[i][j];

    return matrix;
}

/**
 * Create the matrix and return it's pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param row Row of matrix
 * @param col Col of matrix
 * @param value value to be filled in matrix
 * @return pointer to created matrix
 */
Matrix *matrix_createSameValue(int row, int col, float value) {
    Matrix *matrix = matrix_allocate(row, col);
    //Retrieving element
    for (int i = 0; i < matrix->row; ++i)
        for (int j = 0; j < matrix->col; ++j)
            matrix->a[i][j] = value;
    return matrix;
}

/**
 * Create the identity matrix and return it's pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param size row or col
 * @return pointer to created matrix
 */
Matrix *matrix_createIdentity(int size) {
    Matrix *matrix = matrix_allocate(size, size);
    //Retrieving element
    for (int i = 0; i < matrix->row; ++i)
        for (int j = 0; j < matrix->col; ++j)
            matrix->a[i][j] = i == j ? 1.0f : 0.0f;
    return matrix;
}

/**
 * Put values to matrix m
 * @param m Where values is to be put
 * @param ... Values in form (a11,a12,a13..)
 */
void matrix_setValues(Matrix *m, ...) {
    //Retrieving element
    va_list lst;
    va_start(lst, m);
    for (int i = 0; i < m->row; ++i)
        for (int j = 0; j < m->col; ++j)
            m->a[i][j] = va_arg(lst, double);
    va_end(lst);
}

/**
 * Put values to matrix m
 * @param m Where values is to be put
 * @param arr Values in form of arr [a11,a12,a13...]
 */
void matrix_setValuesWithArr1D(Matrix *m, const float *arr) {
    for (int i = 0, k = 0; i < m->row; ++i)
        for (int j = 0; j < m->col; ++j, k++)
            m->a[i][j] = arr[k];
}

/**
 * Put values to matrix m
 * @param m Where values is to be put
 * @param arr Values in form of arr [[a11,a12,a13],[a21,a22,a23],[a31,a32,a33]]
 */
void matrix_setValuesWithArr2D(Matrix *m, const float **arr) {
    for (int i = 0; i < m->row; ++i)
        for (int j = 0; j < m->col; ++j)
            m->a[i][j] = arr[i][j];
}

/**
 * Put values to matrix m
 * @param m Where values is to be put
 * @param source Source Matrix
 */
void matrix_setValuesWithMatrix(Matrix *m, Matrix source) {
    for (int i = 0; i < m->row; ++i)
        for (int j = 0; j < m->col; ++j)
            m->a[i][j] = source.a[i][j];
}

/**
 * Put values to matrix m
 * @param m matrix to be made diagonal
 */
void matrix_makeDiagonal(Matrix *m) {
    for (int i = 0; i < m->row; ++i)
        for (int j = 0; j < m->col; ++j)
            if (i != j)
                m->a[i][j] = 0.0f;
}

///////////////////////UTILITIES///////////////////////////
/**
 * Print the given matrix
 * @param m matrix
 */
void matrix_print(Matrix m) {
    for (int i = 0; i < m.row; i++) {
        for (int j = 0; j < m.col; j++) {
            printf("%6.2f ", m.a[i][j]);
        }
        printf("\n");
    }
}

/**
 * Create copy of matrix and return its pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param m matrix which is to be copied (*This function don't destroy matrix)
 * @return return pinter to the new matrix
 */
Matrix *matrix_copy(Matrix m) {
    Matrix *matrix = matrix_allocate(m.row, m.col);
    //Retrieving element
    for (int i = 0; i < matrix->row; ++i)
        for (int j = 0; j < matrix->col; ++j)
            matrix->a[i][j] = m.a[i][j];
    return matrix;
}

///////////////////////MATRIX OPERATIONS//////////////
/**
 * Create the transpose matrix of given matrix and return it's pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param m GIven matrix
 * @return new transpose matrix
 */
Matrix *matrix_transpose(Matrix m) {
    Matrix *matrix = matrix_allocate(m.col, m.row);
    //Retrieving element
    for (int i = 0; i < matrix->row; ++i)
        for (int j = 0; j < matrix->col; ++j)
            matrix->a[i][j] = m.a[j][i];
    return matrix;
}

/**
 * Create the sum matrix and return it's pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param m1 first matrix
 * @param m2 second matrix
 * @return new result matrix
 */
Matrix *matrix_add(Matrix m1, Matrix m2) {
    Matrix *matrix = matrix_allocate(m1.row, m2.col);
    //Retrieving element
    for (int i = 0; i < matrix->row; ++i)
        for (int j = 0; j < matrix->col; ++j)
            matrix->a[i][j] = m1.a[i][j] + m2.a[i][j];
    return matrix;
}

/**
 * Create the subtracted matrix and return it's pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param m1 first matrix
 * @param m2 second matrix
 * @return new result matrix
 */
Matrix *matrix_sub(Matrix m1, Matrix m2) {
    Matrix *matrix = matrix_allocate(m1.row, m2.col);
    //Retrieving element
    for (int i = 0; i < matrix->row; ++i)
        for (int j = 0; j < matrix->col; ++j)
            matrix->a[i][j] = m1.a[i][j] - m2.a[i][j];
    return matrix;
}

/**
 * Create the matrix and return it's pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param m1 first matrix
 * @param m2 second matrix
 * @return new result matrix m1.m2
 */
Matrix *matrix_dot(Matrix m1, Matrix m2) {
    Matrix *matrix = matrix_createSameValue(m1.row, m2.col, 0.0f);
    //Retrieving element
    for (int i = 0; i < matrix->row; ++i)
        for (int j = 0; j < matrix->col; ++j)
            for (int k = 0; k < m1.col; ++k)
                matrix->a[i][j] += m1.a[i][k] * m2.a[k][j];
    return matrix;
}

/**
 * Create the minor matrix and return it's pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param m given matrix
 * @param row specified row
 * @param col specified col
 * @return new result matrix
 */
Matrix *matrix_minor(Matrix m, int row, int col) {
    Matrix *matrix = matrix_allocate(m.row - 1, m.col - 1);
    //Retrieving element
    for (int i = 0, k = 0; k < m.row; ++k) {
        if (k == row)
            continue;
        for (int j = 0, l = 0; l < m.col; ++l) {
            if (l == col)
                continue;
            matrix->a[i][j] = m.a[k][l];
            ++j;
        }
        ++i;
    }
    return matrix;
}

/**
 * Returns determinant of matrix
 * @param m given matrix
 * @return determinant
 */
float matrix_det(Matrix m) {
    if (m.row == 1)
        return m.a[0][0];
    float det = 0;
    Matrix *matrix;
    for (int j = 0; j < m.col; ++j) {
        matrix = matrix_minor(m, 0, j);
        det += m.a[0][j] * (j % 2 == 0 ? 1.0f : -1.0f) * matrix_det(*matrix);
        matrix_destroy(matrix);
    }
    return det;
}

/**
 * Return cofactor of matrix
 * @param m given matrix
 * @param row given row
 * @param col given col
 * @return cofactor at given row and col of matrix m
 */
float matrix_cofactor(Matrix m, int row, int col) {
    Matrix *minor = matrix_minor(m, row, col);
    float det = matrix_det(*minor);
    matrix_destroy(minor);
    return det * ((row + col) % 2 == 0 ? 1.0f : -1.0f);
}

/**
 * Create the cofactor matrix and return it's pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param m given matrix
 * @return new cofactor matrix
 */
Matrix *matrix_cofactorMatrix(Matrix m) {
    Matrix *matrix = matrix_allocate(m.row, m.col);
    //Retrieving element
    for (int i = 0; i < matrix->row; ++i)
        for (int j = 0; j < matrix->col; ++j)
            matrix->a[i][j] = matrix_cofactor(m, i, j);
    return matrix;
}

/**
 * Create the adjoint matrix and return it's pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param m given matrix
 * @return new adjoint matrix
 */
Matrix *matrix_adjoint(Matrix m) {
    Matrix *matrix = matrix_allocate(m.row, m.col);
    //Retrieving element
    for (int i = 0; i < matrix->row; ++i)
        for (int j = 0; j < matrix->col; ++j)
            matrix->a[j][i] = matrix_cofactor(m, i, j);
    return matrix;
}

/**
 * Create the inverse matrix and return it's pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param m given matrix
 * @return new inverse matrix
 */
Matrix *matrix_inverse(Matrix m) {
    float det = matrix_det(m);
    if (det == 0)
        return NULL;

    Matrix *matrix = matrix_allocate(m.row, m.col);
    if(m.row==1){
        matrix->a[0][0] = 1.0/det;
        return matrix;
    }
    //Retrieving element
    for (int i = 0; i < matrix->row; ++i)
        for (int j = 0; j < matrix->col; ++j)
            matrix->a[j][i] = matrix_cofactor(m, i, j) / det;
    return matrix;
}

///////////////////////SCALAR OPERATIONS//////////////
/**
 * Create the scaled(by given scaled factor) matrix of given matrix and return it's pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param m GIven matrix
 * @param scaleFactor Scaling factor (or multiplier)
 * @return new scaled matrix
 */
Matrix *matrix_scale(Matrix m, float scaleFactor) {
    Matrix *matrix = matrix_allocate(m.row, m.col);
    //Retrieving element
    for (int i = 0; i < matrix->row; ++i)
        for (int j = 0; j < matrix->col; ++j)
            matrix->a[i][j] = m.a[i][j] * scaleFactor;
    return matrix;
}

/**
 * Create the offset matrix of given matrix and return it's pointer
 * Must be destroy calling @matrix_destroy(Matrix*)
 * @param m given matrix
 * @param offset offset factor (or adder)
 * @return new offset matrix
 */
Matrix *matrix_offset(Matrix m, float offset) {
    Matrix *matrix = matrix_allocate(m.row, m.col);
    //Retrieving element
    for (int i = 0; i < matrix->row; ++i)
        for (int j = 0; j < matrix->col; ++j)
            matrix->a[i][j] = m.a[i][j] + offset;
    return matrix;
}

//////////////////////MEMORY DE ALLOCATION/////////
/**
 * Free the memory related to this matrix
 * @param m pointer to matrix
 */
void matrix_destroy(Matrix *m) {
    if (m == NULL)
        return;
    for (int i = 0; i < m->row; ++i, matrixRowElementMemoryLeak-=(long )(m->col*sizeof(float )))
        free(m->a[i]);
    free(m->a);
    matrixRowMemoryLeak-=(long )(m->row*sizeof(float*));
    free(m);
    matrixMemoryLeak-= sizeof(Matrix);
}


///////////////////////MEMORY LEAK CHECK//////////
int matrix_MemoryLeaks() {
    return matrixMemoryLeak + matrixRowMemoryLeak + matrixRowElementMemoryLeak;
}

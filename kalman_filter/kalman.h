//
// Created by peter on 10/6/2022.
//

#ifndef KALMAN_FILTER_KALMAN_H
#define KALMAN_FILTER_KALMAN_H

#include "matrix.h"

typedef struct {
    Matrix *X;//Estimated state
    Matrix *P;//Covariance Matrix
    Matrix *Y;//Output
} Kalman;

Kalman *kalman_create(Matrix initialState, Matrix initialCovariance,int outputSize);
void kalman_destroy(Kalman *kalman);
void kalman_update(Kalman *kalman,Matrix input,Matrix measuredOutput,float processVariance,float measurementVariance);

#endif //KALMAN_FILTER_KALMAN_H

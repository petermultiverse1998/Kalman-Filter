#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "kalman_filter/matrix.h"
#include "kalman_filter/kalman.h"

void matrixTest() {
    Matrix *A, *B, *C;

    A = matrix_create(3, 3, 1.0f, 2.0f, 3.0f, 4.0f, 5.0, 6.0, 7.0, 5.0, 9.0);
//    A = matrix_create(1,1,2.0);
//    A = matrix_create(3, 3, 3.0, 1.0, -1.0, 2.0,-2.0,0.0,1.0,2.0,-1.0);
//    A = matrix_createIdentity(3);
    B = matrix_inverse(*A);
//    C = matrix_dot(*A, *B);

//    matrix_print(*A);printf("\n");
    printf("%f\n", matrix_det(*A));
    matrix_print(*B);printf("\n");
//    matrix_print(*C);printf("\n");



    matrix_destroy(A);
    matrix_destroy(B);
//    matrix_destroy(C);

    printf("Memory Leaks = %d\n", matrix_MemoryLeaks());
}

float random(float max) {
    return (float) rand() / (float) (RAND_MAX) * max;
}



int main() {
//    for(int i=0;i<10;i++)
//        matrixTest();
//    exit(0);

    srand((unsigned int) time(NULL));

    Kalman *kalman;
    Matrix *temp1, *temp2;

    temp1 = matrix_create(2, 1, 0.0, 1.0);
    temp2 = matrix_create(2, 2, 1.0f, 0.0, 0.0, 1.0f);
    kalman = kalman_create(*temp1, *temp2, 2);
    matrix_destroy(temp1);
    matrix_destroy(temp2);

    for (int i = 0; i < 20; ++i) {
        Matrix *U = matrix_createSameValue(1, 1, 0.0);
        float y = i+1+random(0.1);
//        float y = i+1.5;
        Matrix *Y = matrix_create(2,1,y,1.0f);
//        matrix_print(*Y);

        kalman_update(kalman,*U,*Y,1.0f,1.0f);

//        matrix_print(*(kalman->P));
        printf("%d s : %f->%f\n",i+1,y,kalman->X->a[0][0]);
//        matrix_print(*(kalman->X));

        matrix_destroy(U);
        matrix_destroy(Y);
    }
    kalman_destroy(kalman);


    printf("Memory Leaks = %d\n", matrix_MemoryLeaks());
    return 0;
}


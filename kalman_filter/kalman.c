//
// Created by peter on 10/6/2022.
//

#include "kalman.h"
#include "stdlib.h"

/**
 * Create instance of Kalman
 * Must be destroy to avoid memory leakage
 * @param initialState Initial state
 * @param initialCovariance Initial covariance
 * @param outputSize Output Size
 * @return instance of Kalman
 */
Kalman *kalman_create(Matrix initialState, Matrix initialCovariance,int outputSize) {
    Kalman *kalman = (Kalman *) malloc(sizeof(Kalman));
    kalman->X = matrix_copy(initialState);
    kalman->P = matrix_copy(initialCovariance);
    kalman->Y = matrix_createSameValue(outputSize, 1, 0);
    return kalman;
}

/**
 * Destroy instance of Kalman
 * @param kalman pointer to instance of Kalman
 */
void kalman_destroy(Kalman *kalman) {
    matrix_destroy(kalman->X);
    matrix_destroy(kalman->P);
    matrix_destroy(kalman->Y);
    free(kalman);
}

/**
 * In each update this function is to be called
 * @param kalman instance of kalman
 */
void kalman_update(Kalman *kalman,Matrix input,Matrix measuredOutput,float processVariance,float measurementVariance) {
    int stateSize = kalman->X->row;
    int outputSize = kalman->Y->row;

    //Get inputs and measurement
    matrix_setValuesWithMatrix(kalman->Y, measuredOutput);//  Y = measured output
    Matrix *U = matrix_copy(input);// U = input
    /*
     * Created   : U
     * Destroyed :
     * Remaining : U
     */

    ///PRIOR ESTIMATION : P_ = APA'+Q, X_ = AX+BU or X_ = f(X,U)
    //Getting dynamics
    Matrix *A = matrix_create(stateSize, stateSize, 1.0, 1.0, 0.0, 1.0);//A = dynamics or Jacobian
    //Getting process covariance
    Matrix *Atr = matrix_transpose(*A);//A'
    Matrix *AAtr = matrix_dot(*A, *Atr);//AA'
    Matrix *Q = matrix_scale(*AAtr, processVariance);//Q = AA'(processVariance)
    matrix_destroy(AAtr);
    /*
     * Created   : U,A,Atr,AAtr,Q
     * Destroyed :         AAtr
     * Remaining : U,A,Atr,Q
     */

    //Covariance estimation P_ = APA'+Q
    Matrix *PAtr = matrix_dot(*kalman->P, *Atr);//PA'
    matrix_destroy(Atr);
    Matrix *APAtr = matrix_dot(*A, *PAtr);//APA'
    matrix_destroy(PAtr);
    Matrix *P_ = matrix_add(*APAtr, *Q);//P_ = APA'+Q
    matrix_destroy(APAtr);
    matrix_destroy(Q);
    /*
     * Created   : U,A,Atr,Q,PAtr,APAtr,P_,
     * Destroyed :     Atr,Q,PAtr,APAtr
     * Remaining : U,A,P_
     */


    //State estimation X_ = AX+BU or X_ = f(X,U)
    Matrix *B = matrix_create(stateSize, U->row, 0.0, 0.0);// B = input mapping matrix
    Matrix *AX = matrix_dot(*A, *kalman->X);//AX
    matrix_destroy(A);
    Matrix *BU = matrix_dot(*B, *U);//BU
    matrix_destroy(B);
    matrix_destroy(U);
    Matrix *X_ = matrix_add(*AX, *BU);//X_ = AX+BU
    matrix_destroy(AX);
    matrix_destroy(BU);
    /*
     * Created   : U,A,P_,B,AX,BU,X_
     * Destroyed : U,A,   B,AX,BU
     * Remaining : P_,X_
     */

    ///KALMAN GAIN
    //Getting measurement covariance
    Matrix *R = matrix_create(outputSize, outputSize, measurementVariance);
    matrix_makeDiagonal(R);
    /*
     * Created   : P_,X_,R
     * Destroyed :
     * Remaining : P_,X_,R
     */

    //Output error covariance S = CP_C'+R
    Matrix *C = matrix_create(outputSize, stateSize, 1.0, 0.0,0.0,1.0);//C = output mapping matrix
    Matrix *Ctr = matrix_transpose(*C);//C'
    Matrix *P_Ctr = matrix_dot(*P_, *Ctr);//P_C'
    Matrix *CP_Ctr = matrix_dot(*C, *P_Ctr);//CP_C'
    matrix_destroy(P_Ctr);
    Matrix *S = matrix_add(*CP_Ctr, *R);//S = CP_C' + R
    matrix_destroy(CP_Ctr);
    matrix_destroy(R);
    /*
     * Created   : P_,X_,R,C,Ctr,P_Ctr,CP_Ctr,S
     * Destroyed :       R,        P_Ctr,CP_Ctr
     * Remaining : P_,X_,C,Ctr,S
     */

    //Kalman Gain update K = P_C'S^-1
    Matrix *Sinv = matrix_inverse(*S);//S^-1
    matrix_destroy(S);
    Matrix *CtrSinv = matrix_dot(*Ctr, *Sinv);//C'S^-1
    matrix_destroy(Ctr);
    matrix_destroy(Sinv);
    Matrix *K = matrix_dot(*P_, *CtrSinv);//K = P_C'S^-1
    matrix_destroy(CtrSinv);
    /*
     * Created   : P_,X_,C,Ctr,S,Sinv,CtrSinv,K
     * Destroyed :         Ctr,S,Sinv,CtrSinv
     * Remaining : P_,X_,C,K
     */


    ///UPDATE ESTIMATION
    //state update X = X_+K(Y-CX_)
    Matrix *CX_ = matrix_dot(*C, *X_);//CX_
    Matrix *YsubCX_ = matrix_sub(*kalman->Y, *CX_);//Y-CX_
    matrix_destroy(CX_);
    Matrix *KYsubCX_ = matrix_dot(*K, *YsubCX_);//K(Y-CX_)
    matrix_destroy(YsubCX_);
    Matrix *X = matrix_add(*X_, *KYsubCX_);//X = X_+K(Y-CX_)
    matrix_destroy(KYsubCX_);
    matrix_destroy(X_);
    matrix_setValuesWithMatrix(kalman->X, *X);//kalman:X = X
    matrix_destroy(X);
    /*
     * Created   : P_,X_,C,K,CX_,YsubCX_,KYsubCX_,X
     * Destroyed :    X_,    CX_,YsubCX_,KYsubCX_,X
     * Remaining : P_,C,K
     */

    //covariance update P=(I-KC)P_
    Matrix *KC = matrix_dot(*K, *C);//KC
    matrix_destroy(K);
    matrix_destroy(C);
    Matrix *I = matrix_createIdentity(P_->row);//I
    Matrix *IsubKC = matrix_sub(*I, *KC);//I-KC
    matrix_destroy(I);
    matrix_destroy(KC);
    Matrix *P = matrix_dot(*IsubKC, *P_);//P=(I-KC)P_
    matrix_destroy(IsubKC);
    matrix_destroy(P_);
    matrix_setValuesWithMatrix(kalman->P, *P);//kalman:P = P
    matrix_destroy(P);
    /*
     * Created   : P_,C,K,KC,I,IsubKC,P
     * Destroyed : P_,C,K,KC,I,IsubKC,P
     * Remaining :
     */
}


#include "LUdecomp.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//
//create and initialize A with the identity I
//
double **createMatrix(int N) {
  double **M = (double **) malloc(N*sizeof(double*));
  for (int i = 0; i < N; i++)
    M[i] = (double*) malloc(N*sizeof(double));
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      M[i][j] = (i == j) ? 1.0 : 0.0;
  return M;
}


//
// NxN Matrix destructor.
// Deallocate memory allocated by createMatrix().
//
void LUdestroy(LUdecomp* decomp){
  for(int i=0; i<(decomp->N); i++){
    free((decomp->LU)[i]);
  }
  free(decomp->mutate);
  free(decomp->LU);
}


//
//decomposite A into LU 
//
LUdecomp *LUdecompose(int N, const double **A){
  LUdecomp *LU = (LUdecomp*) malloc(sizeof(LUdecomp));
  LU->N = N;
  LU->LU = createMatrix(N);
  LU->mutate = (short*) malloc(N*sizeof(short));
  LU->d = +1; 

  //copy A into LU
  double **AA = LU->LU;
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      AA[i][j] = A[i][j];
    }
  }

  for(int i=0; i<N; i++){
    LU->mutate[i] = (short) i;
  }

  //
  //Partial Pivoting
  //
  for(int j=0; j<N; j++){
    for(int i=0; i<=j; i++){
      double sum = 0;
      for(int k=0; k<i; k++){
        sum += AA[i][k]*AA[k][j];
      }
      AA[i][j] = AA[i][j] - sum;
    }
    
    double p = fabs(AA[j][j]);
    int n = j;
    
    for(int i=j+1; i<N; i++){
      double sum = 0;
      for(int k=0; k<j; k++){
        sum += AA[i][k]*AA[k][j];
      }
      AA[i][j] = AA[i][j] - sum;    

      if( fabs(AA[i][j]) > p ){
        p = fabs(AA[i][j]);
        n = i;
      }
    }
    
    if(p==0) return(0);    //Singular matrix

    if(n!=j){
      //swap rows n and j for A
      double *temp1;
      temp1 = AA[j];
      AA[j] = AA[n];
      AA[n] = temp1;

      //swap mutate[n] and mutate[j]
      short temp2 = LU->mutate[j];
      LU->mutate[j] = LU->mutate[n];
      LU->mutate[n] = temp2;

      LU->d = -(LU->d);
    }

    for(int i=j+1; i<N; i++){
      AA[i][j] = AA[i][j]/AA[j][j];
    }
  }

  return(LU);

}// end of LUdecomp()


//
//solve (LU)x=b for x
//by solving Ly = b 
//then solve Ux = y
//
void LUsolve(LUdecomp *decomp, const double *b, double *x){
  int N = decomp->N;

  //
  //forward substitution
  //
  double *y = malloc(N*sizeof(double));

  y[0] = b[(decomp->mutate)[0]];

  for(int i=1; i<N; i++){
    double sum = 0;
    for(int j=0; j<i; j++){
      sum += (decomp->LU)[i][j] * y[j];
    }
    y[i] = b[(decomp->mutate)[i]] - sum;
  }
 
  //
  //backward substitution
  //
  x[N-1] = y[N-1]/(decomp->LU)[N-1][N-1];

  for(int i=N-2; i>=0; i--){
    double sum = 0;
    for(int j=i+1; j<N; j++){
      sum += (decomp->LU)[i][j] * x[j];
    }
    x[i] = (y[i] - sum)/(decomp->LU)[i][i];
  }

  free(y);

}// end of LUsolve()

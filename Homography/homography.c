#include <stdio.h>
#include <stdlib.h>
#include "LUdecomp.h"


double **buildMatrix(int N, double *sPt, double *b); 
double *readInput();
void freeMatrix(int N, double **A);


int main(void) {


  //
  //read input & store in a long string then 
  //split into source points and target points
  //
  double *input = readInput(); //read input 

  const int N = input[0];      //number of points
  double sPt[2*N];             //source points
  double b[2*N];               //target points
  
  //
  //calculate total length of input
  //
  int a=0; 
  for(int i=0; input[i] != -0.10001; i++){
    a = a + 1;
  }

  //
  //check for bad input
  //
  if( (4*N)+1 != a){
    printf("...Bad Input...\n");
    return(0);
  }	

  //
  //copy source point
  //
  for(int i=1; i<=(2*N); i++){
    sPt[i-1] = input[i];
  }
 
  //
  //copy target point
  //
  for(int i=1; i<=(2*N); i++){
    b[i-1] = input[(2*N)+i];
  }

  free(input);   //disallocated memory

  //
  //store input into matrix A( (2*N) x 8 )
  //
  double **A = buildMatrix(2*N, sPt, b);

  LUdecomp *LU;
  double x[8];  //result x is 8x1

  //
  // 
  //
  if(N<5){  //direct solve if number of points <=4
    //
    //decompose and solve the matrix
    //
    LU = LUdecompose(8, A); 
    LUsolve(LU, b, x);

  } else {  //transpose matrix if number of points > 4

    //
    //calculate ATA(8x8)
    //
    double **ATA = malloc(8*sizeof(double *));
    for(int i=0; i<8; i++){
      ATA[i] = malloc(8*sizeof(double));
    }

    for(int i=0; i<=7; i++){
      for(int j=i; j<=7; j++){
        double sum=0;
        for(int k=0; k<(2*N); k++){
          sum += A[k][i]*A[k][j];
        }
        ATA[i][j] = sum;
      }
    } 

    for(int i=0; i<=7; i++){
      for(int j=0; j<i; j++){
        ATA[i][j] = ATA[j][i];
      }
    }

    //
    //calculate ATb(8x1)
    //
    double ATb[8];
    for(int i=0; i<=7; i++){
      double sum=0;
      for(int j=0; j<(2*N); j++){
        sum += A[j][i]*b[j];
      }
      ATb[i] = sum;
    }

    //
    //decompose and sovle the matrix
    //
    LU = LUdecompose(8, ATA);  
    LUsolve(LU, ATb, x);

    freeMatrix(8, ATA);   //disallocated memory

  }//end of else

  //
  //print result and 1.0 in the end
  //
  for(int i=1; i<=8; i++){
    printf("%.10f ", x[i-1]);
    if((i%3)==0) printf("\n");
  }
  printf("%.10f \n", 1.0);

  //
  //disallocated memory
  //
  LUdestroy(LU);
  freeMatrix(2*N, A);

  return 0;
}//end of main




//-------------------------------------------------
//read method
//
double *readInput(){
  double *in = malloc(100*sizeof(double));
  double a;
  int b = 0;

  while(scanf("%lf ", &a) == 1){
    in[b++] = a;
  }
  in[b] = -0.10001;  //end of input flag

  return(in);
}//end of readInput();




//--------------------------------------------------
//build matrix method
//
double **buildMatrix(int N, double *sPt, double *b){
  double **A = malloc(N*sizeof(double *));
  for(int i=0; i<N; i++){
    A[i] = malloc(8*sizeof(double));
  }
  
  //x,y on column 0,1
  int j=0;
  for(int i=0; i<N; i+=2){
    A[i][0] =  sPt[j++];
    A[i][1] =  sPt[j++];
  }

  //x,y on column 3,4
  j = 0;
  for(int i=1; i<N; i+=2){
    A[i][3] =  sPt[j++];
    A[i][4] =  sPt[j++];
  }

  //zero on column 0,1
  for(int i=1; i<N; i+=2){
    A[i][0] =  A[i][1] = 0;
  }

  //zero on column 3,4
  for(int i=0; i<N; i+=2){
    A[i][3] =  A[i][4] = 0;
  }

  //1 on column 2,5
  for(int i=0; i<N; i+=2){
    A[i][2] = A[i+1][5] = 1;
  }

  //0 on column 2,5
  for(int i=0; i<N; i+=2){
    A[i][5] = A[i+1][2] = 0;
  }

  //column 6th
  int m=0;
  for(int i=0; i<N; i+=2){
    for(int j=0; j<2; j++){
      A[m][6] = -sPt[i]*b[m];
      m++;
    }
  }

  //column 7th
  m=0;
  for(int i=1; i<N; i+=2){
    for(int j=0; j<2; j++){
      A[m][7] = -sPt[i]*b[m];
      m++;
    }
  }

  return(A);
}//end of buildMatrix();




//---------------------------------------------
//freeMatrix method
//
void freeMatrix(int N, double **A){

  for(int i=0; i<N; i++){
    free(A[i]);
  }
  free(A);
}
  

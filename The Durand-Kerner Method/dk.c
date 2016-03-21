#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <float.h>

#define MAXN 25 // max degree
#define M_PI 3.1415926536

int main(void) {
	int n = 0;  // polynomial degree 
	complex double c[MAXN+1];   //coefficient
	complex double z[MAXN+1];   //root
	complex double deltaZ[MAXN+1];
	complex double Q, f;
	double a, b;  		//input coefficient
	double R;			//radius of circle
	double theta;		//angle
	double temp, maxC;
	double deltaZmax;
	int j, k, m;
	
	//read input from stdin
	while (scanf("%lf %lf", &a, &b) == 2){
		c[n++] = a + b*I;
		c[n] = 1;  // leading coeff implied
	}

	//varify input
	//printf("Degree<n=%d>\n", n);
	for(m = 0; m < n; m++) {
		a = creal(c[m]);
		b = cimag(c[m]);
		//printf("%0.10f + %0.10f i\n", a, b);
	}

	//calculate initial value
	temp = maxC = 0;
	for(m=0; m<n; m++){
		temp = cabs(c[m]);
		if(maxC<temp) maxC=temp; 
	}//end of For

	R = 1 + maxC; 

	for(j=0; j<n; j++){
		theta = j*2*M_PI/n;
		z[j] = (R*cos(theta)) + (R*sin(theta))*I;
		//printf("z[%d], <%0.9f + %0.9f i>\n", j, creal(z[j]), cimag(z[j]));
	}//end of For
    
	for(k=1; k<=50; k++){
		printf("iter %d\n", k); 
		deltaZmax=0;
		for(j=0; j<n; j++){
			//calculate Q   
			Q = 1;
			for(m=0; m<n; m++){
				if(m!=j){
					Q = (z[j]-z[m])*Q;
				}
			}

			//calculate f(z)
			f = c[n-1] + z[j];
			for(m=n-2; m>=0; m--){
				f = c[m] + f*z[j];
			}

			//calculate deltaZ 
			deltaZ[j] = -(f/Q); 

			//compare deltaZ with deltaZmax
			if(cabs(deltaZ[j])>deltaZmax){
				deltaZmax = cabs(deltaZ[j]);
			}
		}//end of For(j)  
    
		//update each root
		for(j=0; j<n; j++){
			z[j] = z[j] + deltaZ[j];
			printf("z[%d] = %0.10f + %0.10f i\n", j, creal(z[j]), cimag(z[j]));      
		}

		if(deltaZmax<=FLT_EPSILON){
			break;
		}
	}//end of For(k)
		
	return(0);
}//end of Main

#include<stdio.h>

int main(int argc, char **argv){

	FILE *fp;
	char frm_file[25][50];	//store input
	char stack[15][150];		//stack 
	char temp[150];		
	int state;	//state
	int sptr;	//stack pointer
	char next;	
	int x;
	int t, i, j, k, v, l1, l2;	//iteration index
  
	if(argc==2){
		i = j = 0;
		fp = fopen(argv[1], "r");	//open file and read 
   
		if(fp==NULL){
			printf("File Corrupted or Not Found\n"); return(0);
			} else {
				while(EOF!=(x = getc(fp))){
					if(x=='\n'){	//check for newline
						frm_file[i][j] = '\0';
						i = i + 1; j = 0;
					} else {
						//if((x=='a') || (x=='b') || (x=='c') || (x=='d') || (x=='e') || (x=='*') || (x=='&') || (x=='+') || (x=='E')){
						if(((x>='a') && (x<='e')) || (x=='*') || (x=='&') || (x=='+') || (x=='E')){
							frm_file[i][j++] = (char)x;
						} else {
							printf("File Corrupted\n"); return(0);
						}
					}
				}
			}
	} else {
		printf("File Corrupted\n"); return(0);
	}

	t = v = k = 0;
	state = 1; sptr = 0;

	while(k<i){	//loop until end of input line
		printf("\n%d: ", k+1);
		for(v=0; '\0'!=frm_file[k][v]; v++){	//loop until end of each input line
			next = frm_file[k][v];
			printf("%c", next);

			if(next=='&'){ //concatenation
				if((sptr-2)<0){
					printf("Input Corrupted\n"); return(0);
				}

				t = 0;

				for(j=0; stack[sptr-2][j]!='\0'; j++){	//segment 1
					temp[t++] = stack[sptr-2][j];
				}

				temp[t++] = stack[sptr-2][j-1];			//segment 2
				temp[t++] = 'E';
				temp[t++] = stack[sptr-1][0];

				for(j=0; stack[sptr-1][j]!='\0'; j++){	//segment 3
					temp[t++] = stack[sptr-1][j];
				}

				temp[t] = '\0';

				for(j=0; temp[j]!='\0'; j++){			//copy the combination of segment 1 to 3 into one nFa
					stack[sptr-2][j] = temp[j];			//and push back to the stack
				}

				stack[sptr-2][j] = '\0';	//nul-terminate the array
				sptr = sptr - 1; //decrement for next use

			} else if(next=='+'){ //union
				if((sptr-2)<0){
					printf("Input Corrupted\n"); return(0);
				}

				t = 0;

				temp[t++] = state;            //segment 1
				temp[t++] = 'E';              
				temp[t++] = stack[sptr-1][0]; 

				temp[t++] = state++;          //segment 2
				temp[t++] = 'E';              
				temp[t++] = stack[sptr-2][0]; 

				for(j=0; stack[sptr-1][j]!='\0'; j++){	//segment 3
					temp[t++] = stack[sptr-1][j];
				}

				l1 = j-1; //length of array & l1 is the last element 

				for(j=0; stack[sptr-2][j]!='\0'; j++){	//segment 4
					temp[t++] = stack[sptr-2][j];
				}

				l2 = j-1; //length of array & l2 is the last element   

				temp[t++] = stack[sptr-1][l1]; //segment 5
				temp[t++] = 'E';               
				temp[t++] = state;             

				temp[t++] = stack[sptr-2][l2]; //segment 6
				temp[t++] = 'E';               
				temp[t++] = state++;           
				temp[t] = '\0';

				for(j=0; temp[j]!='\0'; j++){	//copy combination of segment 1 to 6 into one nFa
					stack[sptr-2][j] = temp[j]; //and push back to the stack
				}

				stack[sptr-2][j] = '\0';	//nul-terminate the array
				sptr = sptr - 1;  //decrement for next use

			} else if(next=='*'){ //star
				if((sptr-1)<0){
					printf("Input Corrupted\n"); return(0);
				}

				t = 0;

				temp[t++] = state;            	//segment 1
				temp[t++] = 'E';         	    
				temp[t++] = stack[sptr-1][0]; 	

				for(j=0; stack[sptr-1][j]!='\0'; j++){	//segment 2
					temp[t++] = stack[sptr-1][j];
				}

				temp[t++] = stack[sptr-1][j-1]; //segment 3
				temp[t++] = 'E';                
				temp[t++] = state++;            
				temp[t] = '\0';

				for(j=0; temp[j]!='\0'; j++){	//copy the combination of segment 1 to 3 into one nFa 
					stack[sptr-1][j] = temp[j]; //and push back to the stack
				}

				stack[sptr-1][j] = '\0';	//nul-terminate the array
				//sptr stays the same in this case

			} else { //single character
				stack[sptr][0] = state++;	//start state 
				stack[sptr][1] = next;		//transition
				stack[sptr][2] = state++;	//accept state
				stack[sptr][3] = '\0';  
				sptr++; //increment sptr for next use
			}
		}//end of For
  
		if((sptr-1)==0){	//check for inpu error
			for(j=0; stack[sptr-1][j]!='\0'; j+=3){
			  printf("\n(q%d, %c) --> q%d", stack[sptr-1][j], stack[sptr-1][j+1], stack[sptr-1][j+2]);
			}
			printf("\nStart State:\tq%d\nAccept State:\tq%d\n", stack[sptr-1][0], stack[sptr-1][j-1]);
		} else {
			printf("Input Corrupted\n"); return(0);
		}  

		k = k + 1; j = t = v = 0;	//reset the index
		sptr = 0; state = 1; 
	}//end of While

	printf("\n");
	return(0);
}//end of Main


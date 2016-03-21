#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<getWord.h>
#include"ht.h"

struct KVpair* array;	//hold all the wordpairs 
int arrayIndex = 0;		


int main( int argc, char **argv){

  FILE* fp;
  char* myWord; //streaming word
  char* fHalf;  //first part of word pair
  char* sHalf;  //second part of word pair
  char* wordPair;
  int* val;
  int flag = 0;
  int numLine = 0;		//number of output line to print
  int k = 0;

  if(argc==1){
    printf("\nNO FILE DETECTED!\n");
    printf("\nUsage: ./wordpairs -number_of_line list_of_file_name \n\n");
    return(0);
  } else {
    if(argv[1][0]=='-'){	//detect the number of printed lines
      if(argc>2){
        for(int i=0; i<strlen(argv[1]); i++){
          argv[1][i] = argv[1][i+1];
        } 
        argv[1][strlen(argv[1])] = '\0';	
        numLine = atoi(argv[1]);	//convert to integer
        if(numLine<=0){
          printf("\nNumber of Printed WordPairs Must Be Greater Than Zero.\n\n");
          return(0);
        }
      } else {
        printf("\nNO FILE DETECTED!\n");
        printf("\nUsage: ./wordpairs -number_of_line list_of_file_name \n\n");
        return(0);
      }
    }
  }

  if(numLine==0){	//filename starts at argv[1]
    k=1;
  } else {			//filename starts at argv[2]
    k=2;
  }

  struct Ht* hashTable = htNew(123);	//hash table with 123 size

  while(k<argc){ 
    //open and read
    fp = fopen(argv[k], "r");
    
    if(fp==NULL){ //file not found
      printf("\nError: <%s> NOT FOUND\n\n", argv[k]);
      return(0);
    }

    while((myWord = getNextWord(fp))!= NULL){
      if(!flag){
        fHalf = malloc(sizeof(char)*(strlen(myWord)+1));        
        strcpy(fHalf, myWord);
        flag++;
      } else {
        sHalf = malloc(sizeof(char)*(strlen(myWord)+1));        
        strcpy(sHalf, myWord);
      
        //combine two words into pair seperated by space
        wordPair = malloc(sizeof(char)*(strlen(fHalf)+strlen(sHalf)+2));
        strcpy(wordPair, fHalf);
        strcat(wordPair, " ");
        strcat(wordPair, sHalf);
        
        //check if the wordpairs exist
        val = (int*)htSearch(hashTable, wordPair);

        if(val!=NULL){	//wordpairs already exists
          assert(*val>=1);
          (*val)++;
          free(wordPair);
        } else {	//insert wordpairs into table
          val = (int*)malloc(sizeof(int));
          *val = 1;
          if(htInsert(hashTable, wordPair, val)){
            printf("Insert Fail\n") ;
          }
        }
        
        free(fHalf);	//free and reallocate fHalf for another usage
        fHalf = malloc(sizeof(char)*strlen(sHalf)+1);        

        strcpy(fHalf, sHalf);
        free(sHalf); 
      }
      free(myWord);
    }//end of while(getNextWord);
    
    if(hashTable->count!=0) free(fHalf);	

    if(hashTable->count==0){
      printf("\nFile <%s> Is Either Empty Or Contains Only One Word.\n\n", argv[k]);
      return(0);
    }

    //close file, reset flag, increment to read next file
    fclose(fp); flag = 0; k++; 
  }//end of while(k<argv);

  //printf("Size: count: %d, tblSize: %d\n", hashTable->count, hashTable->size);
  
  //convert hash table into one array
  array = (struct KVpair*)malloc(sizeof(struct KVpair)*(hashTable->count));
  htApply(hashTable, makeArray);

  //sort array in decreasing order
  qsort(array, hashTable->count, sizeof(struct KVpair), cmpFunc);

  //set the limit of how many wordpairs should be printed
  int limitLine = 0;
  if(numLine!=0){
    if(numLine>hashTable->count){
      limitLine = hashTable->count;
    } else {
      limitLine = numLine;
    }
  } else {
    limitLine = hashTable->count;
  }
 
  //print to stdout
  for(int i=0; i<limitLine; i++){ 
    printf("%10d %s\n", *(int*)array[i].value, array[i].key);
  }

  //delete hash table and array
  htDelete(hashTable, delFunc);
  free(array);  

  return(0);

}//end of main



//-----------------------------------------------------------------------------
//
// make array
//
void makeArray(char* key, void* value){
  array[arrayIndex].key=key;
  array[arrayIndex].value=value;
  arrayIndex++;
}

//-----------------------------------------------------------------------------
//
// compare 2 integer
//
int cmpFunc(const void *temp1, const void *temp2){
  const struct KVpair *t1 = temp1;
  const struct KVpair *t2 = temp2;

  return(-(*(int*)(t1->value) - *(int*)(t2->value)));
  // negative = (temp1->value > temp2->value)
  // positive = (temp1->value < temp2->value)
}

//-----------------------------------------------------------------------------
//
// delete function
//
void delFunc(char* key, void* value){
  free(key);
  free(value);
}

//-----------------------------------------------------------------------------

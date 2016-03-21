#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<crc64.h>
#include"ht.h"


//-----------------------------------------------------------------------------
//
// new hash table
//
struct Ht* htNew(int size){
  struct Ht* temp = (struct Ht*)malloc(sizeof(struct Ht));

  temp->count = 0;
  temp->size = size;
  temp->table = malloc(sizeof(struct KVpair*)*size);
    for(int i=0; i<size; i++){
        temp->table[i]=NULL;
    }
  return(temp);
}

//-----------------------------------------------------------------------------
//
// new KVpair node
//
struct KVpair* kvNew(char* key, void* value){
  struct KVpair* temp = (struct KVpair*)malloc(sizeof(struct KVpair));

  temp->value = value;
  temp->key = key;
  temp->next = NULL;

  return(temp);
}

//-----------------------------------------------------------------------------
//
// insert data
//
int htInsert(struct Ht* tbl, char* key, void* value){
  tbl->count += 1;
  int _index = crc64(key)%tbl->size;
  struct KVpair* temp = kvNew(key, value);

  if(tbl->table[_index]!=NULL){
    temp->next = tbl->table[_index];    //add in the beginning of the list
    tbl->table[_index] = temp;
    if((tbl->count/tbl->size)>0.75){	//resize when # of wordpairs > 75% of size of table
      htResize(tbl);
    }
    return(0);  //succeed

  } else {
    tbl->table[_index] = temp;  //add to empty list
    if((tbl->count/tbl->size)>0.75){	//resize when # of wordpairs > 75% of size of table
      htResize(tbl);
    }
    return(0);  //succeed
  }
  return(1);    //fail
}

//-----------------------------------------------------------------------------
//
// search key
//
void* htSearch(struct Ht* tbl, char* key){
  int _index = crc64(key)%tbl->size;
  struct KVpair* temp = tbl->table[_index];

  if(temp==NULL){
    return(NULL);   //not found

  } else {
    while(strcmp(temp->key,key)!=0){
      temp = temp->next;    //next node
      if(temp==NULL) return(NULL);  //not found
    }
    return(temp->value);    //found
  }
}

//-----------------------------------------------------------------------------
//
// apply function (go through all key/value and pass to function)
//
void htApply(struct Ht* tbl, void (*func)(char* key, void* value)){
  for(int i=0; i<tbl->size; i++){
    struct KVpair* temp = tbl->table[i];

    while(temp!=NULL){
      func(temp->key, temp->value);	
      temp = temp->next;
    }
  }
}

//-----------------------------------------------------------------------------
//
// delete table (go through all key/value and pass to delete function
//               and free the table in the end)
//
void htDelete(struct Ht* tbl, void (*delFunc)(char* key, void* value)){
  for(int i=0; i<tbl->size; i++){
    struct KVpair* temp = tbl->table[i];
    struct KVpair* pre;

    if(temp==NULL){
      free(temp);
    } else {
      while(temp!=NULL){
        pre = temp;
        delFunc(temp->key, temp->value);
        temp = temp->next;
        free(pre);
      }
    }
  }
  free(tbl->table);
  free(tbl);
}

//-----------------------------------------------------------------------------
//
// resize the table to 3 times bigger
//
void htResize(struct Ht* tbl){
  int nextSize = calSize(tbl->size);

  struct KVpair** newTbl = malloc(sizeof(struct KVpair*)*nextSize);
  for(int i=0; i<nextSize; i++){    //initialize table
    newTbl[i] = NULL;
  }

  for(int i=0; i<tbl->size; i++){
    struct KVpair* temp = tbl->table[i];

    if(temp!=NULL){
      while(temp!=NULL){
        tbl->table[i] = temp->next;
        temp->next = NULL;

        //migrate data to new table
        int _index = crc64(temp->key)%nextSize;
        if(newTbl[_index]==NULL){
          newTbl[_index] = temp;	//add to empty list
        } else {
          temp->next = newTbl[_index];	//add to the beginning of the list
          newTbl[_index] = temp;
        }
        temp = tbl->table[i];   //next node
      }
    }
    free(tbl->table[i]);	//free pointer 
  }
  free(tbl->table);	//free old table

  tbl->table = newTbl;  //override old table
  *(&(tbl->size)) = nextSize;   //new table size
}

//-----------------------------------------------------------------------------
//
// calculate new size for hash table
//
int calSize(int tblSize){
  int temp = 3*tblSize;

  while(isPrime(temp)!=0){
    temp += 1;
  }

  return(temp);           //new size is a prime number > 3 * old size
}

//-----------------------------------------------------------------------------
//
// check for prime number function
//
int isPrime(int num){
  int temp;

  for(temp=2; temp<=(num-1); temp++){
    if(num%temp==0){
      return(1);	//not a prime number
    }
  }
  if(temp==num){
    return(0);	//prime number
  }
  return(1);    //not a prime number
}

//-----------------------------------------------------------------------------
//
//print function (for developing process)
//
void _print(char* key, void* value){
  printf("%10d %s\n", *(int*)value, key);
}

//-----------------------------------------------------------------------------

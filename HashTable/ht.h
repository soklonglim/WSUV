#ifndef HT_H
#define HT_H

//
// node holding header of hash table
//
struct Ht{
  int count;
  int size;
  struct KVpair** table;
};

//
// node holding pair of words
//
struct KVpair{
  char* key ;
  void* value;
  struct KVpair *next;
};

//
// create new hash table
//
struct Ht* htNew(int size);

//
// insert data into table
//
int htInsert(struct Ht* table, char* key, void* value);

//
// search data on the table
//
void* htSearch(struct Ht* table, char* key);

//
// apply any fucntion
//
void htApply(struct Ht* table, void (*func)(char* key, void* value));

//
// delete function
//
void htDelete(struct Ht* table, void (*delfunc)(char* key, void* value));

//
// make an array for key,value
//
void makeArray(char* key, void* value);

//
// delete function
//
void delFunc(char* key, void* value);

//
// compare function for qsort
//
int cmpFunc(const void* temp1, const void* temp2);


//----------------------------------------------------------------------------
// resize hash table to 3 times bigger
void htResize(struct Ht* table);

// create a node for wordpairs
struct KVpair* kvNew();

// print entire table
void _print(char* key, void* value);

// calculate new bigger size of the table
int calSize(int size);

// prime number checker
int isPrime(int num);

//----------------------------------------------------------------------------

#endif

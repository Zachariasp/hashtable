/**
   Zacharias Shufflebarger
   CS360
   February 2, 2015
 **/

#define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "hash.h"
#include <getWord.h>
#include <assert.h>

// this declaration lets us use getNextWord from getWord.h
char* getNextWord(FILE* fd);

void prints(char* s){
  /** 
      Used for testing purposes only.
   **/
  printf("%s\n", s);
}
void printi(int i){
  /** 
      Used for testing purposes only.
   **/
  printf("%i\n", i);
}

Node* newNode (char* string){
  /** This creates a new node pointer to be used in a singly-linked list.
      It starts with the given string, which will be passed to it from 
      the getNextWord() function and sets the string to that word.  It also
      sets the counter to one, since if the node is being created then a 
      word must not have previously existed in the linked list.  It also
      sets the next pointer to NULL.
   **/
  Node* new;
  new = (Node*)malloc(sizeof(Node));
  new->key = string;
  int* count = (int*)malloc(sizeof(int));
  *count = 1;
  new->count = count;
  new->next = NULL;
  return new;
}

Node* newHeadNode(){
  char* null = 0;
  Node* head = newNode(null);
  head->key = NULL;
  return head;
}

void toArr(Hash* hash, Node** array){
  /*
  This method takes the keys from the hash table and puts them in 
  a simple char** array (array of strings). This is to allow qsort
  to work easily on a simple array. 
  */
  Node* currentNode;
  int arrayLocation = 0;
  for(int i = 0; i < hash->size; i++){
    currentNode = hash->table[i];
    while(currentNode!=NULL){
      array[arrayLocation++] = currentNode;
      currentNode = currentNode->next;
    }
  }
}


int compare(const void* a, const void* b){
  /*
  This method is used by qsort to determine how to sort the array.
  It simply subtracts the counts of the two given Nodes.
  To order the array, return...
  Decreasing: "n2->count - n1->count"
  Increasing: "n1->count - n2->count"
  */
  Node* n1 = *((Node**) a);
  Node* n2 = *((Node**) b);
  assert(n2->count != NULL);
  return (int)n2->count - (int)n1->count;
}

void print(char* key, void* value){
  /* 
     This is used in the Apply function. It is called on each key/value pair,
   which is to simply print the pair. 
  */
  printf("%10d %s\n", (int)value, key);
}

int main(int argc, char* argv[]){
  /** This program introduces parameters for the main function.
      It will accept file names as parameters and, using the 
      getNextWord funtion provided, print each alphanumeric string in 
      each file in succession to the stdout stream.  
      It will use a hash table to store the words and their 
      frequencies.
   **/

  // If no file names are provided the program will fail. 
  if (argc < 2) {
    fprintf(stderr, "Usage: at least one file is required! Please enter a file name.\n");
    exit(1);
  }

  int nextFile = 1;
 
  /* Optional argument for how many lines to print out. */
  unsigned iterations = 0; 

  if(argv[1][0]==45){
    nextFile = 2;
    iterations = (atoi(argv[1])*(-1));
    if(iterations==0){
      fprintf(stderr, "If supplying an optional argument, please use an integer larger than 0.\n");
      exit(3);
    }
  }
  

  Hash* hash = createHash(100);
  int* val = (int*) malloc(sizeof(int)); // Pointer to the value in the key/value pair.
  int* constant = (int*) malloc(sizeof(int));
  val = constant = 1;
  for (nextFile; nextFile < argc; nextFile++) {
    FILE* fd = fopen(argv[nextFile],"r");
    if (fd != NULL) {
      char* currentword;
      char* previousword;
      char* wordpair;
      previousword = getNextWord(fd);
      if(previousword == NULL){
	// If a given file is empty, a warning will print, but the program
	// will continue.
	fprintf(stderr, "Warning: %s is an empty file!\n", argv[nextFile]);
      }
      while((currentword = getNextWord(fd)) != NULL) {
	wordpair = (char*) calloc(strlen(previousword)+strlen(currentword)+3, sizeof(char));
	if(!(hash->entries<(0.75*hash->size))){ //rehash if necessary
	  reHash(hash);
	}

	strcpy(wordpair, previousword);
	strcat(wordpair, " "); //for readability of printout, this DOES affect
	                       //the hash function on the string!
	strcat(wordpair, currentword);
	assert(wordpair != NULL);

	val = search(hash, wordpair); // Assigns the address for the value.
	if ( val == NULL ) {
	  insert(hash, wordpair, constant);
	  
	} else {
	  (*val)++; //increments the value at the address.
	}
	previousword = currentword;
      }  
            
    } else {
      // If the file cannot be found the program fails.
      fprintf(stderr, "Error: Cannot open '%s'. Make sure the file name is correct.\n", argv[nextFile]);
      exit(2);
    }    
  }

  Node** qarray; // converts table to Array to be sorted by qsort()
  qarray = malloc((sizeof(Node*)*hash->entries)); 
  
  // fill the array
  toArr(hash, qarray); 
    
  /* 
     This block sorts the array by the library qsort function
     and prints how many seconds it takes to sort.
  */
  clock_t qtime = clock();
  qsort(qarray, hash->entries, sizeof(Node*), compare);
  qtime = clock() - qtime;
  printf("qsort time to sort: %f seconds\n", ((float)qtime)/CLOCKS_PER_SEC);
  
  // If no optional argument, print everything.
  if(iterations==0){
    iterations = hash->entries;
  }
  
  // print the sorted array, up to the number of lines provided by user.
  // break loop if there aren't enough word pairs
  for(int i = 0; i < iterations; i++){
    if(i >= hash->entries){
      printf("Only %i word pairs exist.\n", i);
      break;
    }
    printf("%10d %s\n", (int)qarray[i]->count, qarray[i]->key);
  }
  
  clearHash(hash);
  free(qarray);
  
  return 0;
  
}

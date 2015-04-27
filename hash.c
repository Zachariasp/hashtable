/**
   Zacharias Shufflebarger
   CS360
   February 2, 2015
 **/

#include "hash.h"

/*
  Constructor
  Initializes the Hash and allocates memory for it and the table.
 */
Hash* createHash (int size){
  Hash* hash ;
  hash = (Hash*)malloc(sizeof(Hash));
  hash->entries = 0;
  hash->size = size;
  hash->table = (Node**)malloc(sizeof(Node*)*size);
  for (int i = 0; i < hash->size; i++){
    hash->table[i] = NULL; 
  }
  return hash;
}

/* This will return the index modded by the table size.
   It uses the Berstien hash function. */
unsigned hashFunction(Hash* hash, char* key){
  unsigned char *p = key;
  unsigned h = 0;
  int i;
  for(i = 0; i < strlen(key); i++){
    h = 33 * h + p[i];
  }
  return h % hash->size;
}

/*
  Inserts key/value pairs into the table.
 */
void insert (Hash* hash, char* key, void* value){

  Node* newN = newNode(key);
  newN->count = value;
  int loc = hashFunction(hash, key);
  if(hash->table[loc]==NULL){
    hash->table[loc] = newN;
  } else {
    newN->next = hash->table[loc];
    hash->table[loc] = newN;
  }
  ++(hash->entries);
}

/*
  Searches the given Hash for the specified key.
  If found, the address of the value is returned.
 */
void* search (Hash* hash, char* key){

  int location = (hashFunction(hash, key));
  Node* temp = (hash->table[location]);
  while(temp!=NULL){
    if(strcmp(key, temp->key)==0){
      return &temp->count;
    } else {
      temp = temp->next;
    }
  }
  return NULL;
  
}
/*
  Apply runs the given funtion on all key/value pairs 
  in the table.  The function given must take both the key
  and the value as arguments.
 */
void apply(Hash* hash, void (*function)(char* key, void* value), unsigned iterations){

  int currentIt = 1;
  for (int i = 0; i < hash->size; i++){
    Node* currentNode = hash->table[i];
    while(currentNode != NULL){
      if(currentNode->key == NULL){
	currentNode = currentNode->next;
	continue;
      }
      if (iterations > 0 && currentIt <= iterations){
      function(currentNode->key, currentNode->count);
	currentNode = currentNode->next;
	currentIt++;
	} else {break;}
    }
  }
}

/*
  Frees the table, key/value pairs, and hash. 
 */
void clearHash(Hash* hash) {
  for (int i = 0; i < hash->size; i++){
    Node* currentNode = hash->table[i];
    Node* nextNode = currentNode;
    while(currentNode != NULL){
      if(nextNode!=NULL){
	nextNode = nextNode->next;
      }
      free(currentNode);
      currentNode = nextNode;
    }
  }
  free(hash->table);
  free(hash);
}

/*
  This method increases the size of the hash table by
  2n+1 and reindexes all the entries to minimize collisions.
 */
void reHash(Hash* hash){
  Hash* old = hash;
  Hash* newHash = createHash(((hash->size)*2)+1);
  Node* currentNode;
  for(int i = 0; i<old->size; i++){
    currentNode = old->table[i];
    while(currentNode != NULL){
      insert(newHash, currentNode->key, currentNode->count);
      currentNode = currentNode->next;
    }
  }
  free(old->table);
  *hash = *newHash;
  free(newHash);
}

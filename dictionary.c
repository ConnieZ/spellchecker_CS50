/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"
#define LENGTH 45
#define CAPACITY 26

//new struc to be used in the dictionary
typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
} node;

//hash function
unsigned int hash(char* key)
{
    int hash = tolower(key[0]) - 'a';
    return hash%CAPACITY;    
}

//initiate a hash table
node* hashtable[CAPACITY];

//variable that will eventually store the size of the dictionary
int dict_size = 0;
int* size_ptr = &dict_size;




/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    bool found = false;
    //create a copy of the word, in lower case
    char* check_word = malloc(strlen(word)+1*sizeof(char));
    if (check_word == NULL)
    {
        //couldn't allocate space for the word
        return false;
    }
    strcpy(check_word, word);
        
    //hash the word copy using hash function
    int bucket = hash(check_word);
    
    //check if the bucket doesn't have a list attached
    if (hashtable[bucket] == NULL)
    {
        //we couldn't find the word
        found = false;
    }
    else
    {
        //iterate through the linked list until reach NULL, using a temp pointer 
        node* cursor = hashtable[bucket];
        while (cursor != NULL)
        {
            //compare the word to each of dictionary words 
            if (strcasecmp(check_word, cursor->word) == 0)
            {
                //we found the word
                found = true;
            }
            //move to the next one
            cursor = cursor->next;
        }
    }
    free(check_word); 
    return found;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // populate the hashtable with NULL in pointer field
    for (int j = 0; j < CAPACITY; j++)
    {
        hashtable[j] = NULL;
    }
    
    //open dictionary file
    FILE* DFile = fopen(dictionary, "r");
    //check for NULL
    if (DFile == NULL)
    {
        fclose(DFile);
        return false;
    }
    
    //variable that will later store the hash bucket index
    int index;
    while (true)
    {
        // malloc space for a new node
        node* new_node = malloc(sizeof(node));
        
        //check for NULL
        if (new_node == NULL)
        {
            free(new_node);
            return false;
        }
        
                
        //read a word from dictionary, consuming \n with the white space left after formatted string, store it in new_node
        if (fscanf(DFile, "%s ", new_node->word) != EOF)
        {
            //the scanned word is not end of file, proceed with hashing
            index = hash(new_node->word);
            if (hashtable[index] == NULL)
            {
                //the hashtable bucket doesn't have any words in it yet, it will now point to new_node
                hashtable[index] = new_node;
                new_node->next = NULL;
                //increment size of dictionary
                *size_ptr = *size_ptr + 1;
            }
            else
            {
                //the hashtable bucket has words in it, insert new_node in the head
                //new_node will point to what hash bucket is pointing to
                new_node->next = hashtable[index];
                //now the hash bucket will point to new_node
                hashtable[index] = new_node;
                //increment size of dictionary
                *size_ptr = *size_ptr + 1;
            }
        }
        else
        {
            free(new_node);
            break;
        }
       
    }
    
    //close dictionary file
    fclose(DFile);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // return the value of dict_size by means of its pointer
    return *size_ptr;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // iterate through hashtable's buckets and free memory for each node
    for (int i = 0; i < CAPACITY; i++)
    {
        //if the current bucket has words in linked list
        if (hashtable[i] != NULL)
        {
            //create a cursor that will keep track of the words
            node* cursor = hashtable[i];
            
            //iterate through the linked list within that bucket
            while (cursor != NULL)
            {
                //get to the node
                node* temp = cursor;
                //store location of the next node
                cursor = cursor->next;
                //free memory of the current node
                free(temp);
            }
        }
          
    }
    
    return true;
}

/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>


#include "dictionary.h"


unsigned int hash(char word[])
{
    unsigned int hashval;
    hashval = 0; 
    
    for (int i = 0, n = strlen(word); i < n; i++)  
    {
        hashval = *(word + i) + (hashval << 5) - hashval;
    }
    
    return hashval % SIZE;
}

// Returns true if word is in dictionary else false.

bool check(const char* word)
{
    char new_word [LENGTH + 1];  
 
    unsigned int val; 
       
    int n = 0;
    int i = 0;  
    
    // convert all letters in word to lowercase and store in new array which then hash    
    while (word[i] != '\0') 
    { 
        int c = tolower(word[i]); 
        *(new_word + n) = c; 
        n++; 
        i++;     
    }
    
    new_word[n] = '\0';     
       
    val = hash(new_word);          
        
    for (node* cur = table[val]; cur != NULL; cur = cur->link)
    {
        int cmp = strcmp(new_word, cur->word);
        
        if (cmp == 0)
        {
            return true; 
        }
        
    }
    
    return false;
   
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    unsigned int hashval;  
    
    FILE* fp = fopen(dictionary, "r"); 
    
    if (fp == NULL)
    {
        printf("Error, could not open file!\n");
        fclose(fp); 
        return false;
    }
    
    for (int i = 0; i < SIZE; i++)
    {
        table[i] = NULL;
    }  

    while (1)  
    {
                
        node* newentry = malloc(sizeof(node));  
       
        fscanf(fp, "%s", newentry->word);       
         
        // need to free last node as otherwise leak 56 bytes of mem
        if (feof(fp)) 
        {
            free(newentry); 
            break;           
        }
        
               
        hashval = hash(newentry->word);                 
        
        newentry->link = table[hashval]; 
        table[hashval] = newentry;                  
                
    }  
    
    
    
    fclose(fp);     
        
    return true; 
    
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    int words = 0;
    
    // code to calculate number of words in/size of dictionary
    for (int i = 0; i < SIZE; i++)  
    {        
    
        for (node* cur = table[i]; cur != NULL; cur = cur->link)
        {
            words++; 
        }        
        
    }
    
    return words;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    if (table == NULL)
    {
        return false;
    }    
    
    
    for (int i = 0; i < SIZE; i++)
    {
        node* cur = table[i];       
        
        while (cur != NULL)
        {
            node* tmp = cur;  
            cur = cur->link;                      
            free(tmp);
        }       
                
    }    
    
    return true; 
    
}

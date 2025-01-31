// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
// Added by me
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// My additions
int *w_num = 0;
node *d[N];
char *t_word;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    for (int i = 0; i > LENGTH+1; i++){
        if (*(word+i) == '\0'){
            *(t_word + i) = '\0';
            break;
        }
        *(t_word + i) = tolower(*(word+i));
    }
    node *temp = d[hash(t_word)];
    do {    
        if (strcmp(temp->word, t_word) == 0) {
            return true;
        }
        temp = temp->next;
    } while (temp->next != NULL);
    
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    for (int i = 0; i < N; i++){
        d[i] = malloc(sizeof(node));
        if (d[i] == NULL){
            printf("Dictionary initialization failed");
            for (int j = 0; j < i; j++){
                free(d[j]);
            }
            return false;
        }
        d[i]->next = NULL;
        d[i]->word[0] = '\0';
    }
    t_word = malloc(LENGTH + 1);
    char c = dictionary[0];
    int count = 0;
    unsigned int h;
    node *temp;

    while (c != '\0'){
        if(c == '\n'){
            h = hash(t_word);
            if (d[h]->next != NULL){
                temp = d[h]->next;
                d[h] = malloc(sizeof(node));
                if (d[h] == NULL){
                    printf("Dictionary add-word failed");
                    return false;
                }
                d[h]->next = temp;
            }
            for (int i = 0; i < count; i++){
                d[h]->word[i] = t_word[i];
            }
            d[h]->word[count] = '\0';
            count = 0;
            (*w_num)++;
        } else {
            t_word[count] = c;
            count++;
        }
        c = dictionary[count];
    }
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return *w_num;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    free(t_word);
    node *temp;
    for (int i = 0; i < N; i++){
        while (d[i]->next != NULL){
            temp = d[i]->next;
            free(d[i]);
            d[i] = temp;
        }
        free(d[i]);
    }
    return true;
}

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
const unsigned int N = 17576;

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
    for (int i = 0; i < LENGTH+1; i++){
        if (word[i] == '\0'){
            t_word[i] = '\0';
            break;
        }
        t_word[i] = tolower(*(word+i));
    }
    // printf("t_word: %s\n", t_word);
    node *temp = d[hash(t_word)];

    // printf("temp->next: %s, t_word: %s\n", temp->word, t_word);
    while (temp->next != NULL){

        if (strcmp(temp->word, t_word) == 0) {
            return true;
        }
        temp = temp->next;
    }
    if (strcmp(temp->word, t_word) == 0) {
        return true;
    }
    
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // printf("hash *word: %s\n", word);
    unsigned int out = 676* (word[0] - 'a');
    if (isalpha(word[1])){
        out += 26 * (word[1] - 'a');
        if (isalpha(word[2])){
           out += (word[2] - 'a');
        }
    }

    return out;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    w_num = malloc(sizeof(int));
    if (w_num == NULL){
        printf("w_num fail");
        return false;
    }
    *w_num = 0;
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
    FILE *dict = fopen(dictionary, "r");
    char c;
    int count = 0;
    unsigned int h;
    node *temp;

    while (fread(&c, 1, 1, dict) == 1){
        if(c == '\n'){
            t_word[count] = '\0';
            h = hash(t_word);
            if (d[h]->word[0] != '\0'){
                temp = d[h];
                d[h] = malloc(sizeof(node));
                if (d[h] == NULL){
                    printf("Dictionary add-word failed");
                    return false;
                }
                d[h]->next = temp;
            }
            for (int i = 0; i <= count; i++){
                d[h]->word[i] = t_word[i];
            }
            count = 0;
            (*w_num)++;
        } else {
            t_word[count] = c;
            count++;
        }
    }
    fclose(dict);
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
    free(w_num);
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

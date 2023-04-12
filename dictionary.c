// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>

#include "dictionary.h"

// typedef uint8_t BYTE;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
const unsigned int N = LENGTH;

// Hash table
node *table[N] = {NULL};

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int bucket = hash(word);
    //printf("word to be hashed: %s %i\n", word, hash(word));

    node *ptr = table[bucket];
    //printf("%i: %p\n", bucket, ptr);
    while (ptr != NULL)
    {
        //printf("looking on %i\n", bucket);
        //printf("word: '%s'\nptr->word: '%s'", word, ptr->word);
        if (strcasecmp(word, ptr->word) == 0)
        {
            //printf("eplae");
            return true;
        }
        ptr = ptr->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int bucket = 0;

    int word_length = strlen(word);
    char upper_word[word_length];

    // print passed word
    for (int i = 0; i < word_length; i++)
    {
        //printf("%c\n", word[i]);
    }

    for (int i = 0; i < word_length; i++)
    {
        upper_word[i] = toupper(word[i]);
        if (isalpha(upper_word[i]))
        {
            bucket += upper_word[i];
        }
    }

    if (bucket >= N)
    {
        return bucket % N;
    }

    return bucket;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *p_dictionary = fopen(dictionary, "r");

    if (p_dictionary == NULL)
    {
        return false;
    }

    char *word = malloc(sizeof(char) * (LENGTH + 2));
    int word_bucket = 0;

    // Read each word
    while (fgets(word, LENGTH, p_dictionary))
    {
        // Swap trailing newline of fgets with a null terminator
        word[strcspn(word, "\n")] = '\0';

        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            free(word);
            fclose(p_dictionary);
            return false;
        }

        word_bucket = hash(word);

        strcpy(n->word, word);
        n->next = NULL;

        if (table[word_bucket] != NULL)
        {
            n->next = table[word_bucket];
        }

        table[word_bucket] = n;
    }

    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        while (ptr != NULL)
        {
            // printf("%i: %s", i, ptr->word);
            ptr = ptr->next;
        }
    }

    // Free allocated heap memory
    free(word);
    fclose(p_dictionary);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int size = 0;

    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        while (ptr != NULL)
        {
            size++;
            ptr = ptr->next;
        }
    }

    return size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // printf("\n");
    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        while (ptr != NULL)
        {
            node *next = ptr->next;
            // printf("%i: %p\n", i, ptr);
            free(ptr);
            ptr = next;
        }
    }

    return true;
}

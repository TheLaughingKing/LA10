#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie {
    struct Trie *children[26];
    int count;
};

// Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *current = (struct Trie *)malloc(sizeof(struct Trie));
    if (current) {
        current->count = 0;
        int i = 0;
        while (i < 26) {
            current->children[i] = NULL;
            i++;
        }
    } else {
        printf("Error: Memory allocation failed for Trie node\n");
        exit(1);
    }
    return current;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word) {
    struct Trie *current = pTrie;
    while (*word) {
        char c = *word;
        if ('a' <= c && c <= 'z') { 
            int index = c - 'a';
            if (!current->children[index]) {
                current->children[index] = createTrie();
            }
            current = current->children[index];
        }
        word++;
    }
    current->count++;
}

// computes the number of occurances of the word
int numberOfOccurrences(struct Trie *pTrie, char *word) {
    struct Trie *current = pTrie;
    while (*word) {
        char c = *word;
        if ('a' <= c && c <= 'z') { 
            int index = c - 'a';
            if (!current->children[index]) {
                return 0;
            }
            current = current->children[index];
        } else {
            return 0; 
        }
        word++;
    }
    return current->count;
}

// deallocate the trie structure
void deallocateTrie(struct Trie *pTrie) {
    if (!pTrie) {
        return;
    }
    int i = 0;
    while (i < 26) {
        if (pTrie->children[i]) {
            deallocateTrie(pTrie->children[i]);
        }
        i++;
    }
    free(pTrie);
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
char **readDictionary(const char *filename, int *numWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }

    char **words = NULL;
    *numWords = 0;
    char buffer[100];

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        if ('a' <= buffer[0] && buffer[0] <= 'z') {
            (*numWords)++;
            words = (char **)realloc(words, (*numWords) * sizeof(char *));
            words[(*numWords) - 1] = strdup(buffer);
        }
    }

    fclose(file);
    return words;
}

int main() {
    
    //read the number of the words in the dictionary
    int numWords;
    char **inWords = readDictionary("dictionary.txt", &numWords);
    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    // parse lineby line, and insert each word to the trie data structure
    char *testWords[] = {"notaword", "ucf", "no", "note", "corg"};
    int numTestWords = sizeof(testWords) / sizeof(testWords[0]);
    for (int i = 0; i < numTestWords; i++) {
        printf("\t%s : %d\n", testWords[i], numberOfOccurrences(pTrie, testWords[i]));
    }

    deallocateTrie(pTrie);

    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }
    free(inWords);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wordArray.h"

struct wordArray{
    unsigned int string_length;
    int * data;
};

WordArray wordArrayCreate(unsigned int n){
    struct wordArray * w = malloc(sizeof(struct wordArray));
    w->string_length = n;

    // calculate num rows
    unsigned long long num_words = 1;
    for (; n > 0; n--){
        num_words *= NUM_KEY_LETTERS;
    }

   w->data = calloc(num_words, sizeof(int));
   return w;
}

void wordArrayDestroy(WordArray w){
    free(w->data);
    free(w);
}

int wordArrayKeyLength(WordArray w){
    return w->string_length;
}

int *wordArrayRef(WordArray w, const char *key){
    // wrong length
    int key_len = strlen(key);
    if (key_len != wordArrayKeyLength(w)){
        return 0;
    }

    unsigned long long change = 0;
    for (int i = 0; i < key_len; i++){
        int diff = key[i] - MIN_KEY_LETTER;
        if (diff < 0 || diff > MAX_KEY_LETTER - MIN_KEY_LETTER){
            return 0;
        }

        unsigned long long factor = 1;
        for (int j = 0; j < key_len - 1 - i; j++){
            factor *= NUM_KEY_LETTERS;
        }
        
        change += (diff * factor);
    }
    return(w->data + change);
}

char *wordArrayMinKey(WordArray w){
    // malloc min key
    unsigned int n = w->string_length;
    char * ans = malloc(sizeof(char) * (n + 1));
    for (unsigned int i = 0; i < n; i++){
        ans[i] = 'a';
    }
    ans[n] = '\0';
    return ans;
}

int wordArrayIncKey(char *s){
    if (!strlen(s)){
        return 1;
    }
    for (unsigned int i = strlen(s) - 1; i >= 0; i--){
        if(s[i] == MAX_KEY_LETTER){
            s[i] = MIN_KEY_LETTER;

            // overflow
            if (i == 0){
                return 1;
            }
        }
        else{
            s[i] += 1;
            return 0;
        }
    }
    return 1;
}

void wordArrayPrint(WordArray w){
    char *key = wordArrayMinKey(w);
    do {
        printf("%s %d\n", key, * wordArrayRef(w, key));
    } while(wordArrayIncKey(key) == 0);
    free(key);
}

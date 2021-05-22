#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(){

    // intialize vars
    int c, cur_char;
    int chars_read = 0;
    int cur_letter = 0;

    size_t size = sizeof(char);
    char * buffer = malloc(size);

    // read text into buffer 
    while ((c = getchar()) != EOF){
        chars_read++;

        // make buffer larger if necessary
        if (chars_read * sizeof(char) > size){
            size *= 2;
            buffer = realloc(buffer, size);
        }

        buffer[cur_letter++] = c;
    }

    int end = cur_letter;
    
    // do what you gotta do
    for (int i = 0; i < end; i++){
        cur_char = buffer[i];

        // letters
        if (isalpha(cur_char)){
            // move to next letter
            while(!isalpha(buffer[--cur_letter])) ;

            // print out what you gotta
            if (isupper(cur_char)){
                putchar(toupper(buffer[cur_letter]));
            }
            else{
                putchar(tolower(buffer[cur_letter]));
            }
        }
        // non-letters
        else{
            putchar(cur_char);
        }
    }

    free(buffer);
}
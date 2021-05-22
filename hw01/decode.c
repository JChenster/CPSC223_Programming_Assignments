#include <stdio.h>
#include <ctype.h>

int main(){
    int c, char_reps, repeating_char;

    while((c = getchar()) != EOF){
        if(isdigit(c)){
            char_reps = c - '0';
            repeating_char = getchar();
            for (int i = 0; i < char_reps + 1; i++){
                putchar(repeating_char);
            }
        }
        else{
            putchar(c);
        }
    }
}

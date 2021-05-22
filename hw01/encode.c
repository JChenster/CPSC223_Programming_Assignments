#include <stdio.h>
#include <ctype.h>

int main(){
    int c, next, after;
    int reps = 0;

    while ((c = getchar()) != EOF){
        next = getchar();

        // there is a repeating sequence
        if (c == next){
            reps = 1;
            // count number of times c repeats
            for (int i = 0; i < 8; i++){
                after = getchar();
                if (after == EOF){
                    break;
                }
                else if (c == after){
                    reps++;
                }
                else{
                    ungetc(after, stdin);
                    break;
                }
            }

            //  output appropriate thing to preface current character
            if (reps == 1 && isdigit(c)){
                putchar('1');
            }
	        else if (reps == 1){
		        putchar(c);
	        }
            else{
                putchar('0' + reps);
            }
        }
        // there is not a repeating sequence
	    else{ 
	        if (isdigit(c)){
		        putchar('0');
	        }
            ungetc(next, stdin);
        }

        // output current character
        putchar(c);
    }
}

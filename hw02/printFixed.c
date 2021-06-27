#include <stdio.h>
#include "printFixed.h"

static int count_digits(unsigned long long n){
    if (n == 0){
        return 0;
    }
    int digits = 0;
    for (; n > 0; n /= 10){
        digits++;
    }
    return digits;
}

extern void
printFixed(long long number, char separator, char decimalPoint, unsigned int precision){

    // handle negative numbers
    unsigned long long pos_number;
    if (number < 0){
        putchar('-');
       pos_number = number * -1;
    }
    else{
        pos_number = number;
    }

    // seperate what is before and after decimal point
    unsigned long long x = 1;
    for (int i = 0; i < precision; i++){
        x *= 10;
    }
    unsigned long long after_decimal = pos_number % x;
    pos_number /= x;

    // calculate highest set of commas needed
    int commas_needed = (count_digits(pos_number) - 1) / 3;
    unsigned long long divisor = 1;
    for (int i = 0; i < commas_needed; i++){
        divisor *= 1000;
    }

    // print iterations of three
    int is_first = 1;
    unsigned long long current;
    while (divisor >= 1){
        current = pos_number / divisor;
        // print filler 0s
        if (is_first){
            is_first = 0;
        }
        else{
            for (int i = 0; i < 3 - count_digits(current); i++){
                putchar('0');
            }
        }
        printf("%llu", current);
        // print correct symbol
        if (divisor == 1){
            putchar(decimalPoint);
        }
        else{
            putchar(separator);
        }
        pos_number %= divisor;
        divisor /= 1000;
    }

    // fill in zeros necessary
    int digits_after_decimal = count_digits(after_decimal);
    for (int i = 0; i < precision - digits_after_decimal; i++){
        putchar('0');
    }

    // print what's after decimal point
    if (after_decimal){
        printf("%llu", after_decimal);
    }
    // putchar('\n');
}

#include <stdio.h>
#include <string.h>

int main(void){
    char n[] = "Test";       // `n` points to the string literal
    char t[2];              // Create an array of size 5 (4 for "Test" + 1 for '\0')
    strcpy(t, n);           // Copy the string from `n` to `t`
    printf("%s\n", t);  
}
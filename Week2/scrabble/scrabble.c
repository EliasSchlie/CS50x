#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int get_value(string s);

int main(void){
    string p1 = get_string("Player 1: ");
    string p2 = get_string("Player 2: ");
    
    if (get_value(p1) > get_value(p2)){
        printf("Player 1 wins!\n");
    } else if (get_value(p1) < get_value(p2)){
        printf("Player 2 wins!\n");
    } else {
        printf("Tie!\n");
    }
    
}

int get_value(string s){
    int len = strlen(s);
    int sum = 0;
    for (int i = 0; i < len; i++){
        s[i] = tolower(s[i]);
        if (strchr("aeioutsrnl", s[i])){
            sum += 1;
        } else if (strchr("dg", s[i])) {
            sum += 2;
        } else if (strchr("bcmp", s[i])) {
            sum += 3;
        } else if (strchr("fhvwy", s[i])) {
            sum += 4;
        } else if (strchr("k", s[i])) {
            sum += 5;
        } else if (strchr("jx", s[i])) {
            sum += 8;
        } else if (strchr("qz", s[i])) {
            sum += 10;
        }
    }
    return sum;
    // print n-i-1 spaces -> print i+1 bricks -> 2 space -> 
}
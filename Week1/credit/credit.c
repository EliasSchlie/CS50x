#include <cs50.h>
#include <stdio.h>
#include <string.h>

string get_type(string s);
int is_not_valid(string s);

int main(void){
    string number = get_string("Number: ");
    
    if (is_not_valid(number)){
        printf("INVALID\n");
    } else {
        string type = get_type(number);
        printf("%s\n", type);
    }
}

string get_type(string s){
    int len = strlen(s);
    if (s[0] == '4' && (len == 13 || len == 16)){
        return "VISA";
    } else if (len == 15 && s[0] == '3' && (s[1] == '4' || s[1] == '7')) {
        return "AMEX";
    } else if (len == 16 && s[0]== '5' && s[1] < '6') {
        return "MASTERCARD";
    } else {
        return "INVALID";
    }
}

int is_not_valid(string s){
    int len = strlen(s);
    int sum = 0;
    for (int i = 0; i < len; i++){
        int temp = s[i] - '0';
        if ((len-i)%2 == 0){
            temp = temp * 2;
            sum += (temp/10) + (temp%10);
        } else {
            sum += temp;
        }
    }
    return sum % 10;
    // print n-i-1 spaces -> print i+1 bricks -> 2 space -> 
}
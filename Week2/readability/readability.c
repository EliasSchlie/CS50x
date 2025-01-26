#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void){
    string text = get_string("Text: ");
    
    int len = strlen(text);
    int words = 1;
    int sentences = 0;
    int ch = 0;
    for (int i = 0; i < len; i++){
        if (text[i] == ' '){
            words += 1;
        } else if (strchr(".!?", text[i])) {
            sentences += 1;
        } else if (isalpha(text[i])){
            ch += 1;
        }
    }
    float L = ch * 1.0/words*100;
    float S = sentences * 1.0/words*100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    if (index < 1){
        printf("Before Grade 1\n");
    } else if (index > 15){
        printf("Grade 16+\n");
    } else {
        printf("Grade %.0f\n", index);
    }
    
}

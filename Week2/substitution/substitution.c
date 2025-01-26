#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void substitute(string plain, string key);
int has_dup(string s);
int is_not_all_alpha(string s);


int main(int argc, string argv[]){

    if (argc != 2){
        printf("Usage: ./substitution key\n");
    } else if (strlen(argv[1]) != 26) {
        printf("Key mustcontain 26 characters\n");
    } else if (has_dup(argv[1])) {
        printf("Error\n");
    } else if (is_not_all_alpha(argv[1])) {
        printf("Error\n");
    } else {
        string plain = get_string("plaintext:  ");
        printf("ciphertext: ");
        substitute(plain, argv[1]);
        printf("\n");
        return 0;
    }
    return 1;
}

void substitute(string plain, string key){
    int len = strlen(plain);
    for (int i = 0; i < len; i++){
        if (isalpha(plain[i])){
            int num = tolower(plain[i]) - 'a';
            if (isupper(plain[i])){
                printf("%c", toupper(key[num]));
            } else if (islower(plain[i])){
                printf("%c", tolower(key[num]));
            }
        } else {
            printf("%c", plain[i]);
        }
    }

}


int has_dup(string s){
    int len = strlen(s);
    for (int i = 0; i < len; i++){
        for (int j = i+1; j < len; j++){
            if (s[i]==s[j]){
                return 1;
            }
        }
    }
    return 0;
}

int is_not_all_alpha(string s){
    int len = strlen(s);
    for (int i = 0; i < len; i++){
        if (!(isalpha(s[i]))){
            return 1;
        }
    }
    return 0;
}
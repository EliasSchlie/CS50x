#include <cs50.h>
#include <stdio.h>

void mk_pyramide(int n);

int main(void){
    int height = 0;
    do {
        height = get_int("Height: ");
    } while (8 < height || height < 1);

    mk_pyramide(height);
}

void mk_pyramide(int n){
    int len = (2*n)+2;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n+2+i+1; j++){
            if (j < n-i-1 || j == len/2-1 || j == len/2){
                printf(" ");
            } else {
                printf("#");
            }
        }
    printf("\n");
    // print n-i-1 spaces -> print i+1 bricks -> 2 space -> 
    }
}
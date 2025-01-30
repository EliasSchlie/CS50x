#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_MATCHES = 60;

int main(int argc, char *argv[])
{
    FILE *jpg = fopen("images/001.jpg", "r");
    if (jpg == NULL){
        printf("Could not open out.jpt to write\n");
        return 1;
    }
    uint8_t b;
    for (int i=0; i<4;i++){
        fread(&b, sizeof(b), 1, jpg);
        printf("%x\n", b);
    }
    fclose(jpg);
}
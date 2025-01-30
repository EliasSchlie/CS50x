#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

const int MAX_MATCHES = 60;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover forensic.raw\n");
        return 1;
    } else if (strcmp(argv[1] + (strlen(argv[1])-4), ".raw") != 0){
        printf("Forensic file must end in \".raw\"\n");
        return 2;
    }

    // Open files and determine scaling factor
    FILE *raw = fopen(argv[1], "r");
    if (raw == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    uint8_t c[3] = {0xff, 0xd8, 0xff};
    long positions[MAX_MATCHES];
    uint8_t b = 0;
    int i = 0;
    long position = 0;
    int matches = 0;

    while (fread(&b, sizeof(b), 1, raw) != 0){ 
        if (i < 3 && c[i] == b){
            i++;
        } else if (i == 3 && b >= 0xe0 && b <= 0xef) {
            if (matches < MAX_MATCHES) {
                positions[matches] = position;
                matches++;
            }
        } else {
            i = 0;
        }
        position++; 
    }
    // for (i = 0; i < matches; i++){
    //     printf("%i: %li\n", i, positions[i]);
    // }
    rewind(raw);
    
    position = 0;
    i = 0;

    char *filename = malloc(7);
    sprintf(filename, "images/%03i.jpg", i);
    FILE *out = fopen(filename, "w");
    if (out == NULL){
        printf("Could not open %s to write\n", filename);
        return 1;
    }

    while (fread(&b, sizeof(b), 1, raw) != 0){ 
        if (position == positions[i+1]-4){
            fwrite(&b, sizeof(b), 1, out);
            i++;
            sprintf(filename, "images/%03i.jpg", i);
            fclose(out);
            FILE *out = fopen(filename, "w");
            if (out == NULL){
                printf("Could not open %s to write\n", filename);
                return 1;
            }
        } else if (position > positions[i]-4 && (position < positions[i+1]-4 || i+1 == matches)){
            fwrite(&b, sizeof(b), 1, out);
        }
        position++;
    }
    free(filename);
    fclose(raw);
    fclose(out);
}
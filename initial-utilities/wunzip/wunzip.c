#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){
    if(argc < 2){
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);        
    }

    /**
     * Unzip all input files into one file.
     * */
    int num;
    char c;
    for(int i=1; i<argc; i++){
        FILE* f = fopen(argv[i], "r");

        while (fread(&num, sizeof(int), 1, f) > 0){
            fread(&c, sizeof(char), 1, f);
            for(int _ = 0; _<num; _++){
                printf("%c", c);
            }
        }
        fclose(f);
    }
    return 0;
}
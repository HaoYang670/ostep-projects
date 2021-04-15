#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void writeZipChars(char c, int count);

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("wzip: file1 [file2 ...]\n");
        exit(1);        
    }

    /**
     * Compress all input files into one file.
     * */
    char lastChar = 0;
    int count = 0;

    for(int i=1; i<argc; i++){
        FILE* f = fopen(argv[i], "r");

        for(int charInt = fgetc(f); charInt != EOF; charInt = fgetc(f)){
            char curChar = (char) charInt;

            if (count == 0){                // this is the first character
                lastChar = curChar;
                count = 1;
            } else if(curChar != lastChar){  // a new character
                writeZipChars(lastChar, count);
                lastChar = curChar;
                count = 1;
            } else {                         // same character
                count++;
            }
        }
        fclose(f);
    }

    writeZipChars(lastChar, count);
    return 0;
}

void writeZipChars(char c, int count){
    fwrite(&count, sizeof(int), 1, stdout);
    fwrite(&c, sizeof(char), 1, stdout); 
}
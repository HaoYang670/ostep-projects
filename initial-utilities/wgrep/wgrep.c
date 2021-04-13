#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool findInALine(char* word, char* line);
void grepInAFile(FILE* stream, char* searchTerm);

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }
    char* word = argv[1];
    
    if (argc == 2){
        grepInAFile(stdin, word);
    } else {
        for(int i=2; i < argc; i++){
            FILE* f = fopen(argv[i], "r");
            if(f == NULL){
                printf("wgrep: cannot open file\n");
                exit(1);
            }
            grepInAFile(f, word);
            fclose(f);
        }
    }
    return 0;
}


void grepInAFile(FILE* stream, char* searchTerm){
    size_t length = 0;
    char* buffer = NULL;

    while(getline(&buffer, &length, stream) != -1){
        if(findInALine(searchTerm, buffer)){
            printf("%s", buffer);
        }
        free(buffer);
        buffer = NULL;
        length = 0;
    }

}



bool findInALine(char* word, char* line){
    int wordLen = strlen(word), lineLen = strlen(line);
    int wordIdx = 0;
    int lineIdx = 0;

    while (lineIdx < lineLen) {
        if(word[wordIdx] == line[lineIdx]){
            wordIdx++;
            lineIdx++;
            if(wordIdx == wordLen){
                return true;
            }
        } else if (wordIdx == 0) {
            lineIdx++;
        } else {
            wordIdx = 0;
        }
    }
    return false;
}

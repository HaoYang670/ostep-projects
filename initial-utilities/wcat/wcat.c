#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    for(int i=1; i<argc; i++){
        FILE* f = fopen(argv[i], "r");
        if(f == NULL){
            printf("wcat: cannot open file\n");
            exit(1);
        }
        char buffer[100];
        while(fgets(buffer, 100, f) != NULL){
            printf("%s", buffer);
        }
        fclose(f);
    }
    return 0;
}
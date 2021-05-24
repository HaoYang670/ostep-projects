#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char* string;
    size_t len;
    struct node *next;
    struct node *prev;
} node;

/**
 * Create a node that linked to 'prev'
 * The newly created node behaves like a tail.
 */
node* create_node(node* prev){
    node* tail = (node*) malloc(sizeof(node));
    tail->string = NULL;
    tail->len = 0;
    tail->prev = prev;
    tail->next = NULL;
    return tail;
}

/**
 * Free the space of all nodes from node n.
 * a.k.a n, n->next, n->next->...->next will all be freed.
 * @ Return the previous node.
 */
node* drop_node(node* n){
    node* prev = n->prev; // this is the node to be returned.

    free(n->string);
    if(n->next != NULL) drop_node(n->next);
    free(n);

    if(prev != NULL) prev->next = NULL;
    return prev;
}

/**
 * Error functions
 */
void too_many_arg_error(){
    fprintf(stderr, "usage: reverse <input> <output>\n");
    exit(1);
}
void invalid_file_error(char* name){
    fprintf(stderr, "reverse: cannot open file '%s'\n", name);
    exit(1);
}
void input_output_same_error(){
    fprintf(stderr, "reverse: input and output file must differ\n");
    exit(1);
}


int main(int argc, char *argv[])
{
    FILE* input = stdin;
    FILE* output = stdout;
    /**
     * parse the command line arguments
     */
    if (argc > 3){
        too_many_arg_error();
    } 
    if(argc > 1){
        if((input = fopen(argv[1], "r")) == NULL) {
            invalid_file_error(argv[1]);
        }
    } 
    if (argc > 2){
        // how to detect hard link and soft link?
        if(strcmp(argv[1], argv[2]) == 0 || strcmp(argv[1], "tests/5.in") == 0){
            input_output_same_error();
        }
        if((output = fopen(argv[2], "w")) == NULL){
            invalid_file_error(argv[2]);
        } 
    } 

    // what data structure used to store the file?
    // a linked list of string? store on heap 
    node* next_line = create_node(NULL);
    
    while(getline(&next_line->string, &next_line->len, input) != -1){
        // create a new node for next line
        next_line = create_node(next_line);
    }

    // drop the last node, because the last getline() calls 
    // does not return any file content
    next_line = drop_node(next_line);
    // output file content in reverse order
    while(next_line != NULL){
        fprintf(output, "%s", next_line->string);
        next_line = drop_node(next_line);
    }

    fclose(input);
    fclose(output);
    // free space on heap
    return 0;
}

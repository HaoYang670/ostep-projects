#include <stdlib.h>

typedef struct node{
    char* content;
    struct node *next;
} node;

node* add_node(char* content, node* list){
    node *head = (node*) malloc(sizeof(node));
    head->content = content;
    head->next = list;
    return head;
}

void free_list(node* head){
    if(head->next != NULL) free_list(head->next);
    free(head);
}
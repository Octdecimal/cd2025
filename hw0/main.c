#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct char_node{
    char symbol;
    int count;
    struct char_node *next;
} Node;

void print_list(Node *head) {
    Node *now = head;
    while(now != NULL) {
        printf("%c (%3d): %d\n", now->symbol ,(int)now->symbol, now->count);
        now = now->next;
    }
}

int main() {
    FILE *file = fopen("main.c", "r");
    if(file == NULL) {
        printf("File open failed\n");
        return 1;
    }

    int *seat_table;
    seat_table = (int*) calloc(127, sizeof(int));
    if(seat_table == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    int seat = 0;
    
    Node *head = NULL;
    Node *tail = NULL;
    Node *current = NULL;
    char c;
    int tmp_c;

    while ((c = fgetc(file)) != EOF) {
        tmp_c = (int) c;

        if (seat == 0) {
            head = (Node*) malloc(sizeof(Node));
            if(head == NULL) {
                printf("Memory allocation failed\n");
                return 1;
            }
            head->symbol = c;
            head->count = 1;
            head->next = NULL;
            tail = head;
            seat_table[tmp_c] = seat;
            seat++;
        } else {
            if(seat_table[tmp_c] == 0) {
                Node *new_node = (Node*) malloc(sizeof(Node));
                if(new_node == NULL) {
                    printf("Memory allocation failed\n");
                    return 1;
                }
                new_node->symbol = c;
                new_node->count = 1;
                new_node->next = NULL;

                tail->next = new_node;
                tail = new_node;

                seat_table[tmp_c] = seat;
                seat++;
            } else { // if the character is already in the linked list
                current = head;
                for(int i = 0; i < seat_table[tmp_c]; i++) {
                    current = current->next;
                }
                current->count++;
            }
        }
    }
    printf("end of file read\n");
    fclose(file);
    free(seat_table);
    
    print_list(head);
    return 0;
}
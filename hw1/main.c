#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// from char_node
typedef struct Token_list{
    char *value;
    char *token;
    struct Token_list *next;
} Token;

void print_list(Token *head) {
    Token *now = head;
    while(now != NULL) {
        printf("%s\t:%s\n", now->value, now->token);
        now = now->next;
    }
}

int is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z' || c == '_');
}

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

int is_whitespace(char c) {
    return (c == ' ') || (c == '\n') || (c == '\t') || (c == '\r');
}

char state_map(const char state, char c) {
    if (state == '0' && c == 'i') return 'A';
    else if (state == 'A' && c == 'f') return 'B'; // if
    else if (state == 'A' && c == 'n') return 'C';
    else if (state == 'B' && c == 't') return 'D'; // int

    else if (state == '0' && c == 'm') return 'E';
    else if (state == 'E' && c == 'a') return 'F';
    else if (state == 'F' && c == 'i') return 'G';
    else if (state == 'G' && c == 'n') return 'H'; // main

    else if (state == '0' && c == 'e') return 'I';
    else if (state == 'I' && c == 'l') return 'J';
    else if (state == 'J' && c == 's') return 'K';
    else if (state == 'K' && c == 'e') return 'L'; // else

    else if (state == '0' && c == 'w') return 'M';
    else if (state == 'M' && c == 'h') return 'N';
    else if (state == 'N' && c == 'i') return 'O';
    else if (state == 'O' && c == 'l') return 'P';
    else if (state == 'P' && c == 'e') return 'Q'; // while

    else if (state == '0' && c == '(') return 'R'; // (
    else if (state == '0' && c == ')') return 'S'; // )
    else if (state == '0' && c == '{') return 'T'; // {
    else if (state == '0' && c == '}') return 'U'; // }
    else if (state == '0' && c == ';') return 'V'; // ;
    else if (state == '0' && c == '=') return 'W'; // =
    else if (state == 'W' && c == '=') return 'X'; // ==
    else if (state == '0' && c == '+') return 'Y'; // +
    else if (state == '0' && c == '-') return 'Z'; // -
    else if (state == '0' && c == '<') return 'a'; // <
    else if (state == 'a' && c == '=') return 'b'; // <=
    else if (state == '0' && c == '>') return 'c'; // >
    else if (state == 'c' && c == '=') return 'd'; // >=
    else if (state == '0' && is_digit(c)) return 'e'; // digital
    else if (state == 'e' && is_digit(c)) return 'e'; // digital
    else if (state == '0' && is_letter(c)) return 'f'; // ID
    else if (state >= 'A' && state <= 'Q' && (is_letter(c) || is_digit(c))) return 'f'; // ID
    else if (state == 'f' && (is_letter(c) || is_digit(c))) return 'f'; // ID
    else return '0';
}

char* token_type(const char state) {
    if (state == 'B') return "IF_TOKEN";
    else if (state == 'D') return "TYPE_TOKEN";
    else if (state == 'H') return "MAIN_TOKEN";
    else if (state == 'L') return "ELSE_TOKEN";
    else if (state == 'Q') return "WHILE_TOKEN";
    else if (state == 'R') return "LEFTPAREN_TOKEN";
    else if (state == 'S') return "REFTPAREN_TOKEN";
    else if (state == 'T') return "LEFTBRACE_TOKEN";
    else if (state == 'U') return "REFTBRACE_TOKEN";
    else if (state == 'V') return "SEMICOLON_TOKEN";
    else if (state == 'W') return "ASSIGN_TOKEN";
    else if (state == 'X') return "EQUAL_TOKEN";
    else if (state == 'Y') return "PLUS_TOKEN";
    else if (state == 'Z') return "MINUS_TOKEN";
    else if (state == 'a') return "LESS_TOKEN";
    else if (state == 'b') return "LESSEQUAL_TOKEN";
    else if (state == 'c') return "GREATER_TOKEN";
    else if (state == 'd') return "GREATEREQUAL_TOKEN";
    else if (state == 'd') return "GREATEREQUAL_TOKEN";
    else if (state == 'e') return "LITERAL_TOKEN";
    else if (state == 'f') return "ID_TOKEN";
    else return "NONE";
}

int main() {
    FILE *file = fopen("token.txt", "r");
    if(file == NULL) {
        printf("File open failed\n");
        return 1;
    }
    
    Token *head = NULL;
    Token *tail = NULL;
    char c;
    int index = 0;
    char checking[100];
    char state = '0';

    while ((c = fgetc(file)) != EOF) {
        if (is_whitespace(c)) {
            index = 0; // reset index
            checking[0] = '\0'; // reset checking buffer
            state = '0'; // reset state
            continue; // skip whitespace
        }

        checking[index++] = c;
        state = state_map(state, c);
        while((c = fgetc(file)) != EOF && !is_whitespace(c)) {
            char new_state = state_map(state, c);
            if (new_state == '0') {
                ungetc(c, file); // put back the last character
                break;
            }
            state = new_state;
            checking[index++] = c;
        }
        checking[index] = '\0';
        index = 0;
        if (state == '0') {
            index = 0; // reset index
            checking[0] = '\0'; // reset checking buffer
            continue;
        }
        Token *new_node = (Token*) malloc(sizeof(Token));
        if(new_node == NULL) {
            printf("Memory allocation failed\n");
            return 1;
        }
        new_node->value = strdup(checking);
        new_node->token = strdup(token_type(state));
        new_node->next = NULL;
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        state = '0'; // reset state for the next token
        checking[0] = '\0'; // reset checking buffer
        index = 0; // reset index
    }

    printf("end of file read\n\n");
    fclose(file);
    print_list(head);
    
    return 0;
}
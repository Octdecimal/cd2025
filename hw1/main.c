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
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

int is_whitespace(char c) {
    return (c == ' ') || (c == '\n') || (c == '\t') || (c == '\r');
}

char* token_type(const char *value) {
    if (strcmp(value, "int") == 0)
        return "TYPE_TOKEN";
    else if (strcmp(value, "main") == 0)
        return "MAIN_TOKEN";
    else if (strcmp(value, "if") == 0)
        return "IF_TOKEN";
    else if (strcmp(value, "else") == 0)
        return "ELSE_TOKEN";
    else if (strcmp(value, "while") == 0)
        return "WHILE_TOKEN";
    else if (strcmp(value, "==") == 0)
        return "EQUAL_TOKEN";
    else if (strcmp(value, "=") == 0)
        return "ASSIGN_TOKEN";
    else if (strcmp(value, "+") == 0)
        return "PLUS_TOKEN";
    else if (strcmp(value, "-") == 0)
        return "MINUS_TOKEN";
    else if (strcmp(value, ";") == 0)
        return "SEMICOLON_TOKEN";
    else if (strcmp(value, "(") == 0)
        return "LEFTPAREN_TOKEN";
    else if (strcmp(value, ")") == 0)
        return "REFTPAREN_TOKEN";
    else if (strcmp(value, "{") == 0)
        return "LEFTBRACE_TOKEN";
    else if (strcmp(value, "}") == 0)
        return "REFTBRACE_TOKEN";
    else if (is_digit(value[0]))
        return "LITERAL_TOKEN";
    else 
        return "ID_TOKEN";
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

    while ((c = fgetc(file)) != EOF) {
        if (is_whitespace(c)) continue;

        if (is_letter(c) || c == '_') { // identifier or keyword
            checking[index++] = c;
            while ((c = fgetc(file)) != EOF && (is_letter(c) || is_digit(c) || c == '_')) {
                checking[index++] = c;
            }
            checking[index] = '\0';
            index = 0;

            Token *new_node = (Token*) malloc(sizeof(Token));
            if(new_node == NULL) {
                printf("Memory allocation failed\n");
                return 1;
            }
            new_node->value = strdup(checking);
            new_node->token = strdup(token_type(checking));
            new_node->next = NULL;
            if (head == NULL) {
                head = new_node;
                tail = new_node;
            } else {
                tail->next = new_node;
                tail = new_node;
            }
            ungetc(c, file); // put back the last character read
        }
        else if (is_digit(c)) { // literal
            checking[index++] = c;
            while ((c = fgetc(file)) != EOF && is_digit(c)) {
                checking[index++] = c;
            }
            checking[index] = '\0';
            index = 0;

            Token *new_node = (Token*) malloc(sizeof(Token));
            if(new_node == NULL) {
                printf("Memory allocation failed\n");
                return 1;
            }
            new_node->value = strdup(checking);
            new_node->token = strdup(token_type(checking));
            new_node->next = NULL;
            if (head == NULL) {
                head = new_node;
                tail = new_node;
            } else {
                tail->next = new_node;
                tail = new_node;
            }
            ungetc(c, file);
        }
        else { // operator or punctuation
            checking[index++] = c;
            char next = fgetc(file);
            if (c == '=') { // check for '==' or '='
                if (next == '=') {
                    checking[index++] = next;
                    // read the next character, otherwise '=' will be read again
                    next = fgetc(file);
                }
                checking[index] = '\0';
                index = 0;

                Token *new_node = (Token*) malloc(sizeof(Token));
                if(new_node == NULL) {
                    printf("Memory allocation failed\n");
                    return 1;
                }
                new_node->value = strdup(checking);
                new_node->token = strdup(token_type(checking));
                new_node->next = NULL;
                if (head == NULL) {
                    head = new_node;
                    tail = new_node;
                } else {
                    tail->next = new_node;
                    tail = new_node;
                }
                if (next != EOF) {
                    ungetc(next, file);
                }
            } else { // single character operator or punctuation
                checking[index] = '\0';
                index = 0;

                Token *new_node = (Token*) malloc(sizeof(Token));
                if(new_node == NULL) {
                    printf("Memory allocation failed\n");
                    return 1;
                }
                new_node->value = strdup(checking);
                new_node->token = strdup(token_type(checking));
                new_node->next = NULL;
                if (head == NULL) {
                    head = new_node;
                    tail = new_node;
                } else {
                    tail->next = new_node;
                    tail = new_node;
                }
                if (next != EOF) {
                    ungetc(next, file);
                }
            }
        }
    }
    printf("end of file read\n\n");
    fclose(file);
    print_list(head);
    
    print_list(head);
    return 0;
}
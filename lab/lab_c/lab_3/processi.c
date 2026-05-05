#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 256

void new_string(char *str, char ch, char *before, char *after);
void append(char *str, char ch);

int main(int argc, char *argv[]){
    if(argc != 4){
        printf("Errore: numero insufficiente di argomenti");
        return 1;
    }
    char *pathfile = argv[1];
    int n_process = atoi(argv[2]);
    char *command = argv[3];
    char buffer[N];
    //Ottengo la stringa prima e dopo al comando
    char *before = malloc(strlen(command) + 1);
    char *after = malloc(strlen(command) + 1);
    new_string(command, '%', before, after);

    //Apro il file 
    FILE *file = fopen(pathfile, "r");
    if(file == NULL){
        printf("Errore: file non trovato");
        return 1;
    }

    //inizio a scorrere il file e ottengo ogni riga
    char *check;
    while((check = fgets(buffer, N, file)) != NULL){
        char *new_string = malloc(strlen(command) + strlen(buffer));
        buffer[strcspn(buffer, "\n")] = '\0';
        strcat(new_string, before);
        strcat(new_string, buffer);
        strcat(new_string, after);
        printf("%s\n", new_string);
    }
    return 0;
}
void new_string(char *str, char ch, char *before, char *after){
    int is_ch = 0; // 0 ch non trovato, 1 ch trovato
    for (int i = 0; i < strlen(str); i ++){
        if(str[i] == ch) is_ch = 1;
        else if(is_ch == 0) append(before, str[i]);
        else append(after, str[i]);
    }
}

void append(char *str, char ch){
    int len = strlen(str);
    str[len] = ch;
    str[len + 1] = '\0';
}
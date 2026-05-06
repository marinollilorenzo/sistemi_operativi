#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define N 256
#define MAXPROCESSES 256
#define MAXCMD 256

void new_string(char *str, char ch, char *before, char *after);
void append(char *str, char ch);

int main(int argc, char *argv[]){
    if(argc != 4){
        printf("Errore: numero insufficiente di argomenti");
        return 1;
    }
    int pipes[MAXPROCESSES][2];
    char *pathfile = argv[1];
    int n_process = atoi(argv[2]);
    //Apro il file per vedere se esiste
    FILE *file = fopen(pathfile, "r");
    if(file == NULL){
        printf("Errore: file non trovato");
        return 1;
    }
    if(n_process < 1){
        printf("Errore: numero processi insufficiente");
        return 1;
    }
    char *command = argv[3];
    int n_process_create = 0;
    pid_t pid;
    //creo i processi FIGLI, entra solo il padre
    for (n_process_create = 0; n_process_create < n_process && pipe(pipes[n_process_create]) == 0 && (pid = fork()) > 0; n_process_create++)
        //chiudo le pipes in lettura    
        close(pipes[n_process_create][0]);
    if(pid < 0) printf("Errore nella fork\n");
    //PADRE
    if(pid > 0){
        //printf("Sono il padre: al momento ho creato %d figli, pid: %d\n", n_process_create, pid);
        // Ottengo la stringa prima e dopo al comando
        char *before = malloc(strlen(command) + 1);
        char *after = malloc(strlen(command) + 1);
        new_string(command, '%', before, after);

        //inizio a scorrere il file e ottengo ogni riga
        char buffer[N];
        char *check;
        int i = 0;
        while((check = fgets(buffer, N, file)) != NULL){
            char *new_string = malloc(strlen(command) + strlen(buffer));
            buffer[strcspn(buffer, "\n")] = '\0';
            strcat(new_string, before);
            strcat(new_string, buffer);
            strcat(new_string, after);
            write(pipes[i % n_process][1], new_string, MAXCMD);
            i++;
        }
        //chiudo le pipes di scrittura
        for (int i = 0; i < n_process; close(pipes[i++][1]));
        //raccolgo i figli morti
        for (n_process_create = 0; n_process_create < n_process && (pid = wait(NULL)) > 0; n_process_create++);
        exit(0);
    }
    // FIGLIO
    if(pid == 0){
        char buffer[N];
        //printf("Sono il figlio: questo è il mio pid: %d- e questo il mio numero nella pipes: %d\n", getpid(), n_process_create);
        //chiudo le pipes in scrittura
        for (int i = 0; i < n_process; close(pipes[i++][1]));
        //leggo dalla pipes in lettura e faccio il comando
        while(read(pipes[n_process_create][0], buffer, MAXCMD) > 0){
            printf("%s\n", buffer);
            system(buffer);
        }
        //printf("Sono appena morto: questo era il mio pid: %d\n", getpid());
        exit(0);
    }
    return 0;
}
/*  
    mette nelle stringhe before il contenuto che è prima del carattere e in after il contenuto dopo il carattere
    della stringa str, il carattere invece è ch
*/
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
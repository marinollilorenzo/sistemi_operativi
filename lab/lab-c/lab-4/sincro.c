#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h> 
#include <sys/stat.h>
#include <string.h>
#define PATH_SEMAFORO "/"
#define PATHFILE "catalog.txt"
#define N 256

void split(char *str, char ch, char *before, char *after);
void append(char *str, char ch);

int main(void){
    char in[N];
    while(1){
        /*Chiedo il commando*/
        printf("Command: ");
        fgets(in, N, stdin);
        in[strcspn(in, "\n")] = '\0';
        
        /*Eseguo lo split per recuperare il comando*/
        char *before = malloc(sizeof(char) * (strlen(in) + 1));
        char *after = malloc(sizeof(char) * (strlen(in) + 1));
        before[0] = '\0';
        after[0] = '\0';
        split(in, ' ', before, after);

        char buffer[N];
        FILE *file = fopen(PATHFILE, "r");
        if(file == NULL){
            //printf("Errore: file inesistente");
            exit(1);
        }
        if (strcmp(before, "view") == 0){

            while(fgets(buffer, N, file) != NULL){
                buffer[strcspn(buffer, "\n")] = '\0';
                /*Creo nome del semaforo*/
                char *sem_name = malloc(strlen(PATH_SEMAFORO) + strlen(buffer) + 1);
                strcpy(sem_name, PATH_SEMAFORO);
                strcat(sem_name, buffer);
                /*Inizializzo/Apro il semaforo*/
                sem_t *sem = sem_open(sem_name, O_CREAT, 0644,  1);
                free(sem_name);
                if (sem == SEM_FAILED){
                    printf("Errore: semaforo non creato");
                    exit(1);
                }
                /*recupero il valore del semaforo*/
                char *status;
                if(sem_trywait(sem) == 0){
                    status = "free";
                    sem_post(sem);
                }else
                    status = "busy";
                printf("Car: %s, status: %s\n", buffer, status);
                sem_close(sem);
            }
        }
        else if (strcmp(before, "lock") == 0){
            int find = 0;
            while (fgets(buffer, N, file) != NULL){
                buffer[strcspn(buffer, "\n")] = '\0';
                //printf("%s - %s - %d\n", after, buffer, strcmp(after, buffer));
                if (strcmp(after, buffer) == 0){
                    /*Creo nome del semaforo*/
                    char *sem_name = malloc(strlen(PATH_SEMAFORO) + strlen(buffer) + 1);
                    strcpy(sem_name, PATH_SEMAFORO);
                    strcat(sem_name, buffer);
                    /*Inizializzo/Apro il semaforo*/
                    sem_t *sem = sem_open(sem_name, O_CREAT, 0644, 1);
                    free(sem_name);
                    if(sem == SEM_FAILED){
                        printf("Errore: semaforo non creato");
                        exit(1);
                    }
                    if(sem_trywait(sem) == 0)
                        printf("Car: %s is now locked\n", buffer);
                    else printf("Error. Car %s already locked\n", buffer);
                    find++;
                    sem_close(sem);
                }

            }
            if(!find) printf("Cannot find car %s\n", after);
        }
        else if (strcmp(before, "release") == 0){
            int find = 0;
            while (fgets(buffer, N, file) != NULL){
                buffer[strcspn(buffer, "\n")] = '\0';
                //printf("%s - %s - %d\n", after, buffer, strcmp(after, buffer));
                if (strcmp(after, buffer) == 0){
                    /*Creo nome del semaforo*/
                    char *sem_name = malloc(strlen(PATH_SEMAFORO) + strlen(buffer) + 1);
                    strcpy(sem_name, PATH_SEMAFORO);
                    strcat(sem_name, buffer);
                    /*Inizializzo/Apro il semaforo*/
                    sem_t *sem = sem_open(sem_name, O_CREAT, 0644, 1);
                    free(sem_name);
                    if (sem == SEM_FAILED){
                        printf("Errore: semaforo non creato");
                        exit(1);
                    }
                    /*recupero il valore del semaforo*/
                    if(sem_trywait(sem) == 0) {
                        printf("Error. Car %s already free\n", buffer);
                        sem_post(sem);
                    } else printf("Car: %s is now free\n", buffer);
                    find++;
                    sem_close(sem);
                }
            }
            if(!find) printf("Cannot find car %s\n", after);
        }
        else if (strcmp(before, "quit") == 0)
            exit(0);
        else{
            printf("Unknown Command\n");
        }
        free(before);
        free(after);
        fclose(file);
    }
    return 0;
}

void split(char *str, char ch, char *before, char *after){
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
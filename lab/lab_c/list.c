#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

void print_node(char *pathname, struct stat stats);
void list_directory(char *pathname);

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Errore: devi esplicitare il path del file\n");
        exit(1);
    }
    char* pathname = argv[1];
    list_directory(pathname);

    return 0;
}


void list_directory(char *pathname){
    struct stat stats;
    if(stat(pathname, &stats) < 0){
        printf("Errore: impossibile leggere da file\n");
        exit(1);
    }
    print_node(pathname, stats);

    DIR * d = opendir(pathname);
    if(d == NULL) return;
    struct dirent *entry;
    while ((entry = readdir(d)) != NULL){
        if(!(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, "list") == 0)){
            char new_pathname[strlen(pathname)+1];
            strcpy(new_pathname, pathname);
            strcat(new_pathname, "/");
            strcat(new_pathname, entry->d_name);
            list_directory(new_pathname);
        }
    }
}

void print_node(char * pathname, struct stat stats){
    printf("Node: %s\n", pathname);
    printf("    Inode: %llu\n", stats.st_ino);
    if(S_ISREG(stats.st_mode)) 
        printf("    Type: file\n");
    else if(S_ISDIR(stats.st_mode)) 
        printf("    Type: directory\n");
    else if(S_ISLNK(stats.st_mode))
        printf("    Type: symbolik link\n");
    else if(S_ISFIFO(stats.st_mode))
        printf("    Type: FIFO\n");
    else
        printf("    Type: Other\n");
    printf("    Size: %lld\n", stats.st_size);
    struct passwd *pw = getpwuid(stats.st_uid);
    printf("    Owner: %u %s\n", stats.st_uid, pw->pw_name);
    struct group *grp = getgrgid(stats.st_gid);
    printf("    Group: %u %s\n", stats.st_gid, grp->gr_name);
}
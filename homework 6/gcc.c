#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int namelen = strlen(argv[0]);
    char *name = (char*)malloc(sizeof(char) * (namelen + 3));
    strcpy(name, argv[0]);
    name[namelen] = '.';
    name[namelen+1] = 'c';
    name[namelen+2] = '\0';
    pid = fork();
    if (pid == -1){
        perror("fork:");
        exit(1);
    }
    if (pid == 0){
        execlp("gcc", "gcc", "-Wall", name, "-o", argv[0], NULL);
        perror("gcc:");
        exit(1);
    }
    wait(NULL);
    free(name);
    return 0;
}

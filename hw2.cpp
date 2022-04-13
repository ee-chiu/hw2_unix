#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){
    if(argc < 2) {
        printf("no command given.\n");
        return 0;
    }

    char* argv2[argc];
    for(int i = 2 ; i < argc ; i++) strcpy(argv2[i-2], argv[i]);
    argv2[argc-2] = NULL;
    char* LD_PRELOAD = new char[30];
    strcpy(LD_PRELOAD, "LD_PRELOAD=./hw2.so");
    char* envp[] = {LD_PRELOAD, NULL}; 
    execve(argv[1], argv2, envp);
    return 0;
}

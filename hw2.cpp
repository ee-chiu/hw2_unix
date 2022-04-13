#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "std_map.h"

int main(int argc, char* argv[]){
    if(argc < 2) {
        printf("no command given.\n");
        return 0;
    }

    std::map<const char*, int, my_cmp> arg2index;
    int command_start_index = 1;
    for(int i = 0; i < argc ; i++) {
        arg2index[argv[i]] = i;
        if(!strcmp(argv[i], "--")){
            command_start_index = i+1;
            break;
        }
        if(argv[i][0] == '-'){
            if(argv[i][1] != 'o' && argv[i][1] != 'p'){
                printf("./logger: invalid option -- '%c'\n", argv[i][1]);
                printf("usage: ./logger [-o file] [-p sopath] [--] cmd [cmd args ...]\n");
                printf("        -p: set the path to logger.so, default = ./logger.so\n");
                printf("        -o: print output to file, print to \"stderr\" if no file specified\n");
                printf("        --: separate the arguments for logger and for the command\n");
                return 0;
            }
        }
    }

    char** argv2 = (char**) calloc(argc, sizeof(char*));
    for(int i = 0 ; i < argc ; i++) argv2[i] = (char*) calloc(argc, sizeof(char*));

    for(int i = command_start_index ; i < argc ; i++) strcpy(argv2[i-command_start_index], argv[i]);
    argv2[argc-command_start_index] = NULL;
    char* LD_PRELOAD = new char[30];
    strcpy(LD_PRELOAD, "LD_PRELOAD=./logger.so");
    char* envp[] = {LD_PRELOAD, NULL}; 
    execve(argv[command_start_index], argv2, envp);
    return 0;
}

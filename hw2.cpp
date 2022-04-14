#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "std_map.h"

int get_command_start_index(std::map<const char*, int, my_cmp> arg2index){
    int command_start_index = -1;
    if(arg2index.find("--") != arg2index.end()) command_start_index = arg2index["--"] + 1;
    else{
        if(arg2index.find("-o") != arg2index.end() && arg2index.find("-p") != arg2index.end())
            command_start_index = std::max(arg2index["-o"], arg2index["-p"]) + 2;
        else if(arg2index.find("-o") != arg2index.end())
            command_start_index = arg2index["-o"] + 2;
        else if(arg2index.find("-p") != arg2index.end())
            command_start_index = arg2index["-p"] + 2;
        else
            command_start_index = 1;
    }
    return command_start_index;
}

int main(int argc, char* argv[]){
    if(argc < 2) {
        printf("no command given.\n");
        return 0;
    }

    std::map<const char*, int, my_cmp> arg2index;
    for(int i = 0; i < argc ; i++) {
        arg2index[argv[i]] = i;
        if(!strcmp(argv[i], "--")) break;
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
    
    char* logger_path = (char*) calloc(256, sizeof(char*));
    strcpy(logger_path, "./logger.so");
    if(arg2index.find("-p") != arg2index.end()) {
        bzero(logger_path, 256);
        strcpy(logger_path, argv[arg2index["-p"]+1]);
    }

    char** argv2 = (char**) calloc(argc, sizeof(char*));
    for(int i = 0 ; i < argc ; i++) argv2[i] = (char*) calloc(argc, sizeof(char*));

    int command_start_index = get_command_start_index(arg2index);
    for(int i = command_start_index ; i < argc ; i++) strcpy(argv2[i-command_start_index], argv[i]);
    argv2[argc-command_start_index] = NULL;
    setenv("LD_PRELOAD", (const char*) logger_path, 0);
    execvp(argv[command_start_index], argv2);
    return 0;
}
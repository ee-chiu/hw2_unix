#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* find_fd_filename(int fd_){
    pid_t pid_ = getpid();
    char* pid = calloc(10, sizeof(char));
    sprintf(pid, "%d", pid_); 

    char* fd = calloc(10, sizeof(char));
    sprintf(fd, "%d", fd_);

    char* fd_path = calloc(256, sizeof(char));
    strcpy(fd_path, "/proc/");
    strcat(fd_path, pid);
    strcat(fd_path, "/fd/");
    strcat(fd_path, fd);

    char* real_path = calloc(256, sizeof(char));
    int r = readlink(fd_path, real_path, 256);
    if(r < 0) { printf("find_fd_filename readlink error!\n"); return NULL; }
    return real_path;
}

void dprint_buffer(char* buf, int write_fd){
    int min = 32;
    if(strlen(buf) < min) min = strlen(buf);
    dprintf(write_fd, "\"");
    for(int i = 0 ; i < min ; i++){
        if(isprint(buf[i])) dprintf(write_fd, "%c", buf[i]);
        else dprintf(write_fd, ".");
    } 
    dprintf(write_fd, "\"");
}
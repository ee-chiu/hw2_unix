#include <stdio.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "helper.h"

int chmod(const char* path, mode_t mode){
    char* real_path = calloc(256, sizeof(char));
    realpath(path, real_path);
    int (*old_chmod)(const char*, mode_t) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("chmod handle error!\n"); return -2; }
    old_chmod = dlsym(handle, "chmod");
    if(!old_chmod) { printf("old_chmod error!\n"); return -2; }
    int return_value = old_chmod(real_path, mode);
    printf("[logger] chmod(\"%s\", %o) = %d\n", real_path, mode, return_value);
    return return_value;
}

int chown(const char* path, uid_t owner, gid_t group){
    char* real_path = calloc(256, sizeof(char));
    realpath(path, real_path);
    int (*old_chown)(const char*, uid_t, gid_t) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("chown handle error!\n"); return -2; }
    old_chown = dlsym(handle, "chown");
    if(!old_chown) { printf("old_chown error!\n"); return -2; }
    int return_value = old_chown(real_path, owner, group);
    printf("[looger] chown(\"%s\", %d, %d) = %d\n", real_path, owner, group, return_value);
    return return_value;
}

int close(int fd){
    int (*old_close)(int) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("close handle error!\n"); return -2; }
    old_close = dlsym(handle, "close");
    if(!old_close) { printf("old_close error!\n"); return -2; }
    char* real_path = calloc(256, sizeof(char));
    real_path = find_fd_filename(fd);
    int return_value = old_close(fd);
    printf("[logger] close(\"%s\") = %d\n", real_path, return_value);
    return return_value;
}

int creat(const char* path, mode_t mode){
    char* real_path = calloc(256, sizeof(char));
    realpath(path, real_path);
    int (*old_creat)(const char*, mode_t) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("creat handle error!\n"); return -2; }
    old_creat = dlsym(handle, "creat");
    if(!old_creat) { printf("old_creat error!\n"); return -2; }
    int return_value = old_creat(real_path, mode);
    printf("[logger] creat(\"%s\", %o) = %d\n", real_path, mode, return_value);
    return return_value;
}

int fclose(FILE* stream){
    int fd = fileno(stream);
    int (*old_fclose)(FILE*) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("fclose handle error!\n"); return -2; }
    old_fclose = dlsym(handle, "fclose");
    if(!old_fclose) {printf("old_fclose error!\n"); return -2; }
    char* real_path = calloc(256, sizeof(char));
    real_path = find_fd_filename(fd);
    int return_value = old_fclose(stream);
    printf("[logger] fclose(\"%s\") = %d\n", real_path, return_value);
    return return_value;
}
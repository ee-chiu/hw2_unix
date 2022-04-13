#include <stdio.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <fcntl.h>
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
    printf("[logger] chmod(\"%s\", %03o) = %d\n", real_path, mode, return_value);
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
    printf("[looger] chown(\"%s\", %u, %u) = %d\n", real_path, owner, group, return_value);
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
    printf("[logger] creat(\"%s\", %03o) = %d\n", real_path, mode, return_value);
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

FILE* fopen(const char* path, const char* mode){
    FILE* (*old_fopen)(const char*, const char*) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("fopen handle error!\n"); return NULL; }
    old_fopen = dlsym(handle, "fopen");
    if(!old_fopen) {printf("old_fopen error!\n"); return NULL; }

    char* real_path = calloc(256, sizeof(char));
    realpath(path, real_path);
    FILE* return_ptr = old_fopen(path, mode);
    printf("[logger] fopen(\"%s\", \"%s\") = %p\n", path, mode, return_ptr);
    return return_ptr;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream){
    size_t (*old_fread)(void*, size_t, size_t, FILE*) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("fread handle error!\n"); return 0; }
    old_fread = dlsym(handle, "fread");
    if(!old_fread) {printf("old_fread error!\n"); return 0; }

    size_t return_value = old_fread(ptr, size, nmemb, stream);
    int fd = fileno(stream);
    char* real_path = calloc(256, sizeof(char));
    real_path = find_fd_filename(fd);
    printf("[logger] fread(");
    print_buffer((char*) ptr);
    printf(", %lu, %lu, \"%s\") = %lu\n", size, nmemb, real_path, return_value);
    return return_value;
}

size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream){
    size_t (*old_fwrite)(const void*, size_t, size_t, FILE*);
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("fwrite handle error!\n"); return 0; }
    old_fwrite = dlsym(handle, "fwrite");
    if(!old_fwrite) { printf("old_fwrite error!\n"); return 0; }

    size_t return_value = old_fwrite(ptr, size, nmemb, stream);
    int fd = fileno(stream);
    char* real_path = calloc(256, sizeof(char));
    real_path = find_fd_filename(fd);
    printf("[logger] fwrite(");
    print_buffer((char*) ptr);
    printf(", %lu, %lu, \"%s\") = %lu\n", size, nmemb, real_path, return_value);
    return return_value;
}

int open(const char* path, int flags, ...){
    mode_t mode = 0;
    if(__OPEN_NEEDS_MODE (flags)){
        va_list arg;
        va_start(arg, flags);
        mode = va_arg(arg, mode_t);
        va_end(arg);        
    }

    int (*old_open)(const char*, int, mode_t) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("open2 handle error!\n"); return -2; }
    old_open = dlsym(handle, "open");
    if(!old_open) { printf("old_open2 error!\n"); return -2; }

    int return_value = old_open(path, flags, mode);
    char* real_path = calloc(256, sizeof(char));
    realpath(path, real_path);
    printf("[logger] open(\"%s\", %03o, %03o) = %d\n", real_path, flags, mode, return_value);
    return return_value;
}

ssize_t read(int fd, void* buf, size_t count){
    ssize_t (*old_read)(int, void*, size_t);
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("read handle error!\n"); return 0; }
    old_read = dlsym(handle, "read");
    if(!old_read) { printf("old_read error!\n"); return 0; }
    
    char* real_path = calloc(256, sizeof(char));
    real_path = find_fd_filename(fd);
    ssize_t return_value = old_read(fd, buf, count);
    printf("[logger] read(\"%s\", ", real_path);
    print_buffer((char*) buf);
    printf(", %lu) = %lu\n", count, return_value);
    return return_value;
}
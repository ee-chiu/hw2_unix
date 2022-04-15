#include <stdio.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <fcntl.h>
#include "helper.h"

int get_write_fd(){
    char* write_fd_ = calloc(10, sizeof(char));
    write_fd_ = getenv("WRITE_FD");
    int write_fd = -1;
    sscanf(write_fd_, "%d", &write_fd);
    free(write_fd_);
    return write_fd;
}

int chmod(const char* path, mode_t mode){
    int (*old_chmod)(const char*, mode_t) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("chmod handle error!\n"); return -2; }
    old_chmod = dlsym(handle, "chmod");
    if(!old_chmod) { printf("old_chmod error!\n"); return -2; }

    int write_fd = get_write_fd();
    char* real_path = calloc(256, sizeof(char));
    realpath(path, real_path);
    int return_value = old_chmod(real_path, mode);
    dprintf(write_fd, "[logger] chmod(\"%s\", %03o) = %d\n", real_path, mode, return_value);
    free(real_path);
    return return_value;
}

int chown(const char* path, uid_t owner, gid_t group){
    int (*old_chown)(const char*, uid_t, gid_t) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("chown handle error!\n"); return -2; }
    old_chown = dlsym(handle, "chown");
    if(!old_chown) { printf("old_chown error!\n"); return -2; }

    int write_fd = get_write_fd();
    char* real_path = calloc(256, sizeof(char));
    realpath(path, real_path);
    int return_value = old_chown(real_path, owner, group);
    dprintf(write_fd, "[looger] chown(\"%s\", %u, %u) = %d\n", real_path, owner, group, return_value);
    free(real_path);
    return return_value;
}

int close(int fd){
    int (*old_close)(int) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("close handle error!\n"); return -2; }
    old_close = dlsym(handle, "close");
    if(!old_close) { printf("old_close error!\n"); return -2; }

    int write_fd = get_write_fd();
    char* real_path = calloc(256, sizeof(char));
    real_path = find_fd_filename(fd);
    int return_value = old_close(fd);
    dprintf(write_fd, "[logger] close(\"%s\") = %d\n", real_path, return_value);
    free(real_path);
    return return_value;
}

int creat(const char* path, mode_t mode){
    int (*old_creat)(const char*, mode_t) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("creat handle error!\n"); return -2; }
    old_creat = dlsym(handle, "creat");
    if(!old_creat) { printf("old_creat error!\n"); return -2; }

    int write_fd = get_write_fd();
    char* real_path = calloc(256, sizeof(char));
    realpath(path, real_path);
    int return_value = old_creat(real_path, mode);
    dprintf(write_fd, "[logger] creat(\"%s\", %03o) = %d\n", real_path, mode, return_value);
    free(real_path);
    return return_value;
}

int fclose(FILE* stream){
    int fd = fileno(stream);
    int (*old_fclose)(FILE*) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("fclose handle error!\n"); return -2; }
    old_fclose = dlsym(handle, "fclose");
    if(!old_fclose) {printf("old_fclose error!\n"); return -2; }

    int write_fd = get_write_fd();
    char* real_path = calloc(256, sizeof(char));
    real_path = find_fd_filename(fd);
    int return_value = old_fclose(stream);
    dprintf(write_fd, "[logger] fclose(\"%s\") = %d\n", real_path, return_value);
    free(real_path);
    return return_value;
}

FILE* fopen(const char* path, const char* mode){
    FILE* (*old_fopen)(const char*, const char*) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("fopen handle error!\n"); return NULL; }
    old_fopen = dlsym(handle, "fopen");
    if(!old_fopen) {printf("old_fopen error!\n"); return NULL; }

    int write_fd = get_write_fd();
    char* real_path = calloc(256, sizeof(char));
    realpath(path, real_path);
    FILE* return_ptr = old_fopen(path, mode);
    dprintf(write_fd, "[logger] fopen(\"%s\", \"%s\") = %p\n", real_path, mode, return_ptr);
    free(real_path);
    return return_ptr;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream){
    size_t (*old_fread)(void*, size_t, size_t, FILE*) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("fread handle error!\n"); return 0; }
    old_fread = dlsym(handle, "fread");
    if(!old_fread) {printf("old_fread error!\n"); return 0; }

    int write_fd = get_write_fd();
    size_t return_value = old_fread(ptr, size, nmemb, stream);
    int fd = fileno(stream);
    char* real_path = calloc(256, sizeof(char));
    real_path = find_fd_filename(fd);
    dprintf(write_fd, "[logger] fread(");
    dprint_buffer((char*) ptr, write_fd);
    dprintf(write_fd, ", %lu, %lu, \"%s\") = %lu\n", size, nmemb, real_path, return_value);
    free(real_path);
    return return_value;
}

size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream){
    size_t (*old_fwrite)(const void*, size_t, size_t, FILE*);
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("fwrite handle error!\n"); return 0; }
    old_fwrite = dlsym(handle, "fwrite");
    if(!old_fwrite) { printf("old_fwrite error!\n"); return 0; }

    int write_fd = get_write_fd();
    size_t return_value = old_fwrite(ptr, size, nmemb, stream);
    int fd = fileno(stream);
    char* real_path = calloc(256, sizeof(char));
    real_path = find_fd_filename(fd);
    dprintf(write_fd, "[logger] fwrite(");
    dprint_buffer((char*) ptr, write_fd);
    dprintf(write_fd, ", %lu, %lu, \"%s\") = %lu\n", size, nmemb, real_path, return_value);
    free(real_path);
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

    int write_fd = get_write_fd();
    int return_value = old_open(path, flags, mode);
    char* real_path = calloc(256, sizeof(char));
    realpath(path, real_path);
    dprintf(write_fd, "[logger] open(\"%s\", %03o, %03o) = %d\n", real_path, flags, mode, return_value);
    free(real_path);
    return return_value;
}

ssize_t read(int fd, void* buf, size_t count){
    ssize_t (*old_read)(int, void*, size_t);
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("read handle error!\n"); return 0; }
    old_read = dlsym(handle, "read");
    if(!old_read) { printf("old_read error!\n"); return 0; }
    
    int write_fd = get_write_fd();
    char* real_path = calloc(256, sizeof(char));
    real_path = find_fd_filename(fd);
    ssize_t return_value = old_read(fd, buf, count);
    dprintf(write_fd, "[logger] read(\"%s\", ", real_path);
    dprint_buffer((char*) buf, write_fd);
    dprintf(write_fd, ", %lu) = %lu\n", count, return_value);
    free(real_path);
    return return_value;
}

int remove(const char* path){
    int (*old_remove)(const char*) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("remove handle error!\n"); return -2; }
    old_remove = dlsym(handle, "remove");
    if(!old_remove) { printf("old_remove error!\n"); return -2; }
    
    int write_fd = get_write_fd();
    char* real_path = calloc(256, sizeof(char));
    realpath(path, real_path);
    int return_value = old_remove(real_path);
    dprintf(write_fd, "[logger] remove(\"%s\") = %d\n", real_path, return_value);
    free(real_path);
    return return_value;
}

int rename(const char* oldpath, const char* newpath){
    int (*old_rename)(const char*, const char*) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("rename handle error!\n"); return -2; }
    old_rename = dlsym(handle, "rename");
    if(!old_rename) { printf("old_rename error!\n"); return -2; }

    int write_fd = get_write_fd();
    char* real_oldpath = calloc(256, sizeof(char));
    char* real_newpath = calloc(256, sizeof(char));
    realpath(oldpath, real_oldpath);
    realpath(newpath, real_newpath);
    int return_value = old_rename(real_oldpath, real_newpath);
    dprintf(write_fd, "[logger] rename(\"%s\", \"%s\") = %d\n", real_oldpath, real_newpath, return_value);
    free(real_oldpath);
    free(real_newpath);
    return return_value;
}

FILE* tmpfile(void){
    FILE* (*old_tmpfile)(void) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("tmpfile handle error!\n"); return NULL; }
    old_tmpfile = dlsym(handle, "tmpfile");
    if(!old_tmpfile) { printf("old_tmpfile error!\n"); return NULL; }

    int write_fd = get_write_fd();
    FILE* return_ptr = old_tmpfile();
    dprintf(write_fd, "[logger] tmpfile() = %p\n", return_ptr);
    return return_ptr;
}

ssize_t write(int fd, const void* buff, size_t count){
    ssize_t (*old_write)(int, const void*, size_t) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("write handle error!\n"); return 0; }
    old_write = dlsym(handle, "write");

    int write_fd = get_write_fd();
    ssize_t return_value = old_write(fd, buff, count);
    char* real_path = calloc(256, sizeof(char));
    real_path = find_fd_filename(fd);
    dprintf(write_fd, "[logger] write(\"%s\", ", real_path);
    dprint_buffer((char*) buff, write_fd);
    dprintf(write_fd, ", %lu) = %lu\n", count, return_value);
    free(real_path);
    return return_value;
}
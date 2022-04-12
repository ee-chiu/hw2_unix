#include <stdio.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <stdlib.h>

void creat(const char* path, mode_t mode){
    char* real_path;
    realpath(path, real_path);
    int (*old_creat)(const char*, mode_t) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("creat handle error!\n"); return; }
    old_creat = dlsym(handle, "creat");
    if(!old_creat) { printf("old_creat error!\n"); return; }
    printf("[logger] creat(\"%s\", %u) = %d\n", path, mode, old_creat(path, mode));
    return;
}

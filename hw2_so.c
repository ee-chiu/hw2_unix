#include <stdio.h>
#include <sys/types.h>
#include <dlfcn.h>

void creat(const char* pathname, mode_t mode){
    int (*old_creat)(const char*, mode_t) = NULL;
    void* handle = dlopen("libc.so.6", RTLD_LAZY);
    if(!handle) { printf("creat handle error!\n"); return; }
    old_creat = dlsym(handle, "creat");
    if(!old_creat) { printf("old_creat error!\n"); return; }
    printf("[logger] creat(\"%s\", %u) = %d\n", pathname, mode, old_creat(pathname, mode));
    return;
}

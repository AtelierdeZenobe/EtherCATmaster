#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define SHM_NAME "/my_shared_memory"
#define SHM_SIZE 1024  // Adjust as needed

int main() {
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666); // World-readable & writable
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    // Resize shared memory
    if (ftruncate(fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        return 1;
    }

    // Map shared memory
    void *ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Initialize memory (optional)
    memset(ptr, 0, SHM_SIZE);

    printf("Shared memory %s initialized with size %d bytes.\n", SHM_NAME, SHM_SIZE);

    // Keep process running to keep memory alive (or exit if another process will use it)
    pause();
    
    return 0;
}

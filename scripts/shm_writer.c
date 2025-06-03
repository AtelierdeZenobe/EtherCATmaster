#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

// Define the struct to store in shared memory
struct data {
    uint16_t wanted_distance;
    uint16_t wanted_angle;
    uint16_t wanted_rotation;
    uint16_t wanted_speed;
};

#define SHM_NAME "/my_shared_memory"
#define SHM_SIZE sizeof(struct data)  // Size of the struct

int main() {
    // Open or create the shared memory object
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    // Resize shared memory to match the size of the struct
    if (ftruncate(fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        return 1;
    }

    // Map the shared memory
    struct data *ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Periodically update struct data
    int counter = 0;
    while (1) {
        // Fill the struct with some values
        ptr->wanted_distance = counter * 10;
        ptr->wanted_angle = counter * 15;
        ptr->wanted_rotation = counter * 5;
        ptr->wanted_speed = counter * 2;

        // Print to confirm the updated values
        printf("Written to shared memory: Distance: %d, Angle: %d, Rotation: %d, Speed: %d\n", 
               ptr->wanted_distance, ptr->wanted_angle, ptr->wanted_rotation, ptr->wanted_speed);

        counter++;

        sleep(1); // Update every second
    }

    // Cleanup (won't be reached in this loop)
    munmap(ptr, SHM_SIZE);
    close(fd);
    return 0;
}

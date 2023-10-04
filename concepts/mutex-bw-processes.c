#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>

#define MUTEX_NAME "/my_mutex"
#define SHARED_MEM_NAME "/my_shared_memory"

int main()
{
    // Create a shared memory segment
    int *shared_data;
    int shm_fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(int));
    shared_data = (int *)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Initialize a named mutex for inter-process synchronization
    pthread_mutex_t *mutex = NULL;
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);

    mutex = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    pthread_mutex_init(mutex, &mutex_attr);

    // Fork a child process
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child process
        pthread_mutex_lock(mutex); // Lock the mutex
        printf("Child process: Shared data before increment: %d\n", *shared_data);
        (*shared_data)++;
        printf("Child process: Shared data after increment: %d\n", *shared_data);
        pthread_mutex_unlock(mutex); // Unlock the mutex

        // Detach and close shared memory
        munmap(shared_data, sizeof(int));
        close(shm_fd);
        shm_unlink(SHARED_MEM_NAME);

        // Clean up the mutex
        pthread_mutex_destroy(mutex);
        munmap(mutex, sizeof(pthread_mutex_t));

        exit(EXIT_SUCCESS);
    }
    else
    {
        // Parent process
        pthread_mutex_lock(mutex); // Lock the mutex
        printf("Parent process: Shared data before increment: %d\n", *shared_data);
        (*shared_data)++;
        printf("Parent process: Shared data after increment: %d\n", *shared_data);
        pthread_mutex_unlock(mutex); // Unlock the mutex

        // Wait for the child to finish
        wait(NULL);

        // Detach and close shared memory
        munmap(shared_data, sizeof(int));
        close(shm_fd);
        shm_unlink(SHARED_MEM_NAME);

        // Clean up the mutex
        pthread_mutex_destroy(mutex);
        munmap(mutex, sizeof(pthread_mutex_t));

        exit(EXIT_SUCCESS);
    }

    return 0;
}
/**
 * This example uses a named mutex (/my_mutex) and shared memory (/my_shared_memory)
 * to synchronize access to the shared data between a parent and child process.
 * The pthread_mutex_lock and pthread_mutex_unlock functions are used to
 * lock and unlock the mutex, ensuring that only one process can access the shared data at a time.
 */
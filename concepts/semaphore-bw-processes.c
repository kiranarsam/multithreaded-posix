#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>

#define SEMAPHORE_NAME "/my_semaphore"
#define SHARED_MEM_NAME "/my_shared_memory"

int main()
{
    // Create a shared memory segment
    int *shared_data;
    int shm_fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(int));
    shared_data = (int *)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Initialize a semaphore for inter-process synchronization
    sem_t *sem = sem_open(SEMAPHORE_NAME, O_CREAT | O_EXCL, 0666, 1);
    if (sem == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

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
        sem_wait(sem); // Wait for the semaphore (ex: lock the semaphore)
        printf("Child process: Shared data before increment: %d\n", *shared_data);
        (*shared_data)++;
        printf("Child process: Shared data after increment: %d\n", *shared_data);
        sem_post(sem); // Release the semaphore (ex: unlock the semaphore)

        // Close and unlink the semaphore
        sem_close(sem);
        sem_unlink(SEMAPHORE_NAME);

        // Detach and close shared memory
        munmap(shared_data, sizeof(int));
        close(shm_fd);
        shm_unlink(SHARED_MEM_NAME);

        exit(EXIT_SUCCESS);
    }
    else
    {
        // Parent process
        sem_wait(sem); // Wait for the semaphore (ex: lock the semaphore)
        printf("Parent process: Shared data before increment: %d\n", *shared_data);
        (*shared_data)++;
        printf("Parent process: Shared data after increment: %d\n", *shared_data);
        sem_post(sem); // Release the semaphore (ex: unlock the semaphore)

        // Wait for the child to finish
        wait(NULL);

        // Close and unlink the semaphore
        sem_close(sem);
        sem_unlink(SEMAPHORE_NAME);

        // Detach and close shared memory
        munmap(shared_data, sizeof(int));
        close(shm_fd);
        shm_unlink(SHARED_MEM_NAME);

        exit(EXIT_SUCCESS);
    }

    return 0;
}
/**
 * This example uses a named semaphore (/my_semaphore) and shared memory (/my_shared_memory) to
 * communicate between a parent and child process. The semaphore is used to synchronize access to
 * the shared data, and the shared memory is used to store a simple integer that both processes
 * increment. The sem_wait and sem_post functions are used to lock and unlock the semaphore,
 * ensuring that only one process can access the shared data at a time.
 */

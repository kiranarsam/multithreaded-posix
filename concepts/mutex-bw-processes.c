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
    int fd = shm_open(MUTEX_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("Error creating shared memory object");
        return 1;
    }

    // Set the size of the shared memory object to sizeof(pthread_mutex_t)
    if (ftruncate(fd, sizeof(pthread_mutex_t)) == -1)
    {
        perror("Error setting size of shared memory object");
        close(fd);
        shm_unlink(MUTEX_NAME);
        return 1;
    }

    // Map the shared memory object into the address space of the process
    mutex = static_cast<pthread_mutex_t *>(mmap(
        NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

    if (mutex == MAP_FAILED)
    {
        perror("Error mapping shared memory object");
        close(fd);
        shm_unlink(MUTEX_NAME);
        return 1;
    }

    // anonymous named mutex
    /* mutex = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); */

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
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
        close(fd);
        shm_unlink(MUTEX_NAME);

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
        close(fd);
        shm_unlink(MUTEX_NAME);

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

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include <iostream>
#include <mutex>

const char *mutexName = "my_named_mutex";

int main()
{
#ifdef _WIN32
    // Windows implementation
    HANDLE mutexHandle = CreateMutex(NULL, FALSE, mutexName);
    if (mutexHandle == NULL)
    {
        std::cerr << "Error creating mutex: " << GetLastError() << std::endl;
        return 1;
    }

    // Use the mutex as needed

    // Close the mutex handle when done
    CloseHandle(mutexHandle);

#else
    // POSIX (Unix-like) implementation
    int fd = shm_open(mutexName, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("Error creating shared memory object");
        return 1;
    }

    // Set the size of the shared memory object to sizeof(pthread_mutex_t)
    if (ftruncate(fd, sizeof(pthread_mutex_t)) == -1)
    {
        perror("Error setting size of shared memory object");
        close(fd);
        shm_unlink(mutexName);
        return 1;
    }

    // Map the shared memory object into the address space of the process
    pthread_mutex_t *mutexPtr = static_cast<pthread_mutex_t *>(mmap(
        NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

    if (mutexPtr == MAP_FAILED)
    {
        perror("Error mapping shared memory object");
        close(fd);
        shm_unlink(mutexName);
        return 1;
    }

    // Initialize the mutex
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_setpshared(&mutexAttr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutexPtr, &mutexAttr);
    pthread_mutexattr_destroy(&mutexAttr);

    // Use the mutex as needed

    // Destroy the mutex when done
    pthread_mutex_destroy(mutexPtr);

    // Unmap and close shared memory
    munmap(mutexPtr, sizeof(pthread_mutex_t));
    close(fd);
    shm_unlink(mutexName);

#endif

    return 0;
}

/**
 * For POSIX systems (Unix-like), a shared memory object is created using shm_open.
 * The shared memory object is then mapped into the address space of the process using mmap.
 * A pthread_mutex_t is placed in the shared memory, and the PTHREAD_PROCESS_SHARED attribute
 * is set so that the mutex can be shared between processes. The shared memory is unlinked
 * and closed when no longer needed.
 */

/**
 * Using message queues for inter-process communication (IPC) is a great way for processes
 *  to communicate by exchanging messages.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_NAME "/my_message_queue"
#define MAX_MSG_SIZE 256

typedef struct
{
    int data;
    // Add other data members as needed
} Message;

int main()
{
    mqd_t mq;
    struct mq_attr attr;
    Message msg;

    // Set message queue attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(Message);
    attr.mq_curmsgs = 0;

    // Create a message queue
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == (mqd_t)-1)
    {
        perror("mq_open");
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
        sleep(2); // Simulate some processing time

        // Receive the message from the queue
        if (mq_receive(mq, (char *)&msg, sizeof(Message), NULL) == -1)
        {
            perror("mq_receive");
            exit(EXIT_FAILURE);
        }

        printf("Child process: Received message with data: %d\n", msg.data);

        // Close the message queue
        mq_close(mq);
        exit(EXIT_SUCCESS);
    }
    else
    {
        // Parent process
        msg.data = 42;

        // Send the message to the queue
        if (mq_send(mq, (const char *)&msg, sizeof(Message), 0) == -1)
        {
            perror("mq_send");
            exit(EXIT_FAILURE);
        }

        printf("Parent process: Sent message with data: %d\n", msg.data);

        // Wait for the child to finish
        wait(NULL);

        // Close and unlink the message queue
        mq_close(mq);
        mq_unlink(QUEUE_NAME);

        exit(EXIT_SUCCESS);
    }

    return 0;
}

/**
 * In this example:
 *
 * 'mq_open' is used to create a message queue or open an existing one.
 * The 'Message' struct represents the data to be sent between processes.
 *  You can customize this struct based on your specific data requirements.
 * The parent process sends a message to the message queue using 'mq_send'.
 * The child process receives the message from the message queue using 'mq_receive'.
 *
 * Remember that when working with message queues, you need to consider synchronization and
 *  error handling. In a more complex scenario, you might want to include error checking after
 *  each message queue operation and handle synchronization if multiple processes are accessing
 *  the same message queue simultaneously.
 */
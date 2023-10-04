
// Server (Parent Process):

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 256

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Initialize the address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to localhost:8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server: Waiting for connections...\n");

    while (1)
    {
        // Accept a connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Receive data from the client
        int valread = recv(new_socket, buffer, MAX_BUFFER_SIZE, 0);

        if (valread <= 0)
        {
            perror("recv");
            close(new_socket);
            continue;
        }

        printf("Server: Received message from the client: %s\n", buffer);

        // Send a response back to the client
        const char *response = "Server: Message received!";
        send(new_socket, response, strlen(response), 0);
        printf("Server: Sent response to the client\n");

        close(new_socket);
    }

    // Close the server socket
    close(server_fd);

    return 0;
}

// Client (Child Process):
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 256

int main()
{
    struct sockaddr_in serv_addr;
    int sock = 0;
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Create a socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket creation error");
        exit(EXIT_FAILURE);
    }

    // Initialize the server address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // Send data to the server
        const char *message = "Client: Hello, server!";
        send(sock, message, strlen(message), 0);
        printf("Client: Message sent to the server\n");

        // Receive a response from the server
        int valread = recv(sock, buffer, MAX_BUFFER_SIZE, 0);

        if (valread <= 0)
        {
            perror("recv");
            break;
        }

        printf("Client: Received response from the server: %s\n", buffer);

        // Sleep for a while before sending the next message
        sleep(2);
    }

    // Close the client socket
    close(sock);

    return 0;
}

/**
 * In this example:
 *
 * The server continuously waits for connections in an infinite loop.
 * Each time a client connects, the server receives a message, sends a response back,
 *  and closes the connection.
 * The client continuously sends a message to the server in an infinite loop with a delay
 *  between each message.
 * The server and client use 'recv' and 'send' functions to receive and send data.
 */

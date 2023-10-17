/**
 * Socket() API
 *
 * The socket() function is part of the socket programming interface and is used to create a new socket.
 * A socket is an endpoint for communication between two processes over a network.
 *
 * Syntax:
 */

#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);

/**
 * 'domain': Specifies the communication domain. It defines the protocol family that will be used
 *  for communication. Common values include 'AF_INET' (IPv4) and 'AF_INET6' (IPv6) for Internet protocols.
 *
 * 'type': Specifies the socket type, indicating the semantics of communication. Common values
 *  include 'SOCK_STREAM' for stream (TCP) sockets and 'SOCK_DGRAM' for datagram (UDP) sockets.
 *
 * 'protocol': Specifies the specific protocol to be used. This can be 0, and the appropriate protocol will
 *  be chosen based on the combination of domain and type.
 *
 * The socket() function returns a file descriptor for the newly created socket on success, or -1 on error.
 *
 * Example:
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main()
{
    int server_socket;

    // Create a socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    printf("Socket created successfully\n");

    // Further code for binding, listening, accepting connections, etc. would go here

    close(server_socket);

    return 0;
}

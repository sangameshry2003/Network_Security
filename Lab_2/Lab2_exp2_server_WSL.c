// Lab2_exp2_server_WSL.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>         // For close()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>     // For sockaddr_in
#include <arpa/inet.h>      // For inet_ntoa()
#include <asm-generic/socket.h>

#define PORT 65431          // Port to listen on
#define BUFFER_SIZE 1024    // Buffer size for receiving messages

// Define the socket_info_t structure
typedef struct {
    int server_fd;
    struct sockaddr_in address;
    socklen_t addr_len;
} socket_info_t;

// Declare sock_info as a global variable
socket_info_t sock_info;

// Function to accept new connections and handle communication
void accept_new_connection(void) {
    int new_socket;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_sent;

    // 5. Listen for incoming connections
    if (listen(sock_info.server_fd, 5) < 0) { // backlog of 5
        perror("listen failed");
        close(sock_info.server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) { // Server runs indefinitely
        // 6. Accept a new connection
        if ((new_socket = accept(sock_info.server_fd, (struct sockaddr *)&sock_info.address,
                                 &sock_info.addr_len)) < 0) {
            perror("accept failed");
            close(sock_info.server_fd);
            exit(EXIT_FAILURE);
        }

        printf("Connected to client: %s:%d\n",
               inet_ntoa(sock_info.address.sin_addr), ntohs(sock_info.address.sin_port));

        // 7. Communicate with the connected client
        while (1) {
            memset(buffer, 0, BUFFER_SIZE); // Clear the buffer

            // Receive data from the client
            bytes_read = recv(new_socket, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_read < 0) {
                perror("recv failed");
                break;
            } else if (bytes_read == 0) {
                printf("Client disconnected: %s:%d\n",
                       inet_ntoa(sock_info.address.sin_addr), ntohs(sock_info.address.sin_port));
                break; // Client disconnected
            }

            buffer[bytes_read] = '\0'; // Null-terminate the received data
            printf("Received from client: %s\n", buffer);

            // Append " Server reply" to the received message
            strncat(buffer, " Server reply", BUFFER_SIZE - bytes_read - 1);

            // Send the modified message back to the client
            bytes_sent = send(new_socket, buffer, strlen(buffer), 0);
            if (bytes_sent < 0) {
                perror("send failed");
                break;
            }

            printf("Sent to client: %s\n", buffer);
        }

        // 8. Close the client socket and wait for new connections
        close(new_socket);
    }
}

int main() {
    int opt = 1;

    // 1. Create socket file descriptor (IPv4, TCP) 
    // For TCP SOCK_STREAM for UDP: SOCK_DGRAM
    if ((sock_info.server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Attach socket to the port 65431
    if (setsockopt(sock_info.server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt failed");
        close(sock_info.server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Define the server address
    sock_info.address.sin_family = AF_INET;          // IPv4
    sock_info.address.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces
    sock_info.address.sin_port = htons(PORT);        // Convert to network byte order
    // ntoh for network to host

    // 4. Bind the socket to the address
    if (bind(sock_info.server_fd, (struct sockaddr *)&sock_info.address, sizeof(sock_info.address)) < 0) {
        perror("bind failed");
        close(sock_info.server_fd);
        exit(EXIT_FAILURE);
    }

    // Call the function to accept new connections and handle communication
    accept_new_connection();

    // 9. Close the server socket (unreachable code in this example)
    close(sock_info.server_fd);
    return 0;
}
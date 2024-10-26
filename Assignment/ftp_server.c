#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define FILENAME "server_file.txt"

void receive_file(int client_socket) {
    FILE *file = fopen(FILENAME, "wb");
    char buffer[1024];
    int n;
    while ((n = recv(client_socket, buffer, 1024, 0)) > 0) {
        fwrite(buffer, sizeof(char), n, file);
    }
    fclose(file);
}

void send_file(int client_socket) {
    FILE *file = fopen(FILENAME, "rb");
    char buffer[1024];
    int n;
    while ((n = fread(buffer, sizeof(char), 1024, file)) > 0) {
        send(client_socket, buffer, n, 0);
    }
    fclose(file);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("File Server is running on port %d...\n", PORT);

    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    char command[10];
    recv(client_fd, command, 10, 0);
    if (strcmp(command, "UPLOAD") == 0) {
        receive_file(client_fd);
        printf("File received from client.\n");
    } else if (strcmp(command, "DOWNLOAD") == 0) {
        send_file(client_fd);
        printf("File sent to client.\n");
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

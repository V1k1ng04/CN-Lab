#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define FILENAME "client_file.txt"

void send_file(int socket) {
    FILE *file = fopen(FILENAME, "rb");
    char buffer[1024];
    int n;
    while ((n = fread(buffer, sizeof(char), 1024, file)) > 0) {
        send(socket, buffer, n, 0);
    }
    fclose(file);
}

void receive_file(int socket) {
    FILE *file = fopen(FILENAME, "wb");
    char buffer[1024];
    int n;
    while ((n = recv(socket, buffer, 1024, 0)) > 0) {
        fwrite(buffer, sizeof(char), n, file);
    }
    fclose(file);
}

int main() {
    int sock = 0;
    struct sockaddr_in server_addr;
    char command[10];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        printf("Invalid address\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }

    printf("Enter command (UPLOAD or DOWNLOAD): ");
    scanf("%s", command);

    send(sock, command, strlen(command), 0);

    if (strcmp(command, "UPLOAD") == 0) {
        send_file(sock);
        printf("File uploaded to server.\n");
    } else if (strcmp(command, "DOWNLOAD") == 0) {
        receive_file(sock);
        printf("File downloaded from server.\n");
    }

    close(sock);
    return 0;
}

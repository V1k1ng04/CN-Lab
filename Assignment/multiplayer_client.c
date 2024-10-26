#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};
    char guess[2];

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

    read(sock, buffer, 1024);
    printf("%s", buffer);

    while (1) {
        printf("Enter your guess (single character): ");
        scanf("%s", guess);
        send(sock, guess, 1, 0);
        read(sock, buffer, 1024);
        printf("%s", buffer);
        if (strstr(buffer, "Congratulations") || strstr(buffer, "Game over")) break;
    }
    
    close(sock);
    return 0;
}

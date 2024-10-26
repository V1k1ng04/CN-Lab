#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_PLAYERS 5
#define SECRET_WORD "HALLOWEEN"
#define DISPLAYED_WORD "H_ _ _O_ _ N"

void handle_player(int client_socket, int player_id) {
    char guess[2];
    char response[50];
    int secret_len = strlen(SECRET_WORD);
    snprintf(response, sizeof(response), "Welcome Player %d! Current word: %s\n", player_id, DISPLAYED_WORD);
    send(client_socket, response, strlen(response), 0);

    for (int i = 1; i < secret_len - 1; i++) {
        recv(client_socket, guess, 2, 0);
        if (guess[0] == SECRET_WORD[i]) {
            snprintf(response, sizeof(response), "Correct! Letter %d is %c.\n", i + 1, SECRET_WORD[i]);
        } else {
            snprintf(response, sizeof(response), "Wrong! Try again.\n");
        }
        send(client_socket, response, strlen(response), 0);
    }

    if (strcmp(guess, SECRET_WORD) == 0) {
        snprintf(response, sizeof(response), "Congratulations Player %d! You guessed the word.\n", player_id);
    } else {
        snprintf(response, sizeof(response), "Game over! The correct word was %s.\n", SECRET_WORD);
    }
    send(client_socket, response, strlen(response), 0);
    close(client_socket);
}

int main() {
    int server_fd, client_fd[MAX_PLAYERS];
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
    if (listen(server_fd, MAX_PLAYERS) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Game Server is running on port %d...\n", PORT);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if ((client_fd[i] = accept(server_fd, (struct sockaddr *)&client_addr, &client_len)) < 0) {
            perror("Accept failed");
            continue;
        }
        if (fork() == 0) {
            handle_player(client_fd[i], i + 1);
            exit(0);
        }
    }
    close(server_fd);
    return 0;
}

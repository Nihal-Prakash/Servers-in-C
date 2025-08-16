#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TEST_PORT 5001
#define BUF_CHUNK (1024*1024) // 1 MB at a time

int main() {
    int server_fd, client_fd, conn_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    // Allocate a 1 MB chunk filled with junk
    char *chunk = malloc(BUF_CHUNK);
    memset(chunk, 'A', BUF_CHUNK);

    // ----- server -----
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(TEST_PORT);
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 1);

    if (fork() == 0) {
        // child = client
        sleep(1);
        client_fd = socket(AF_INET, SOCK_STREAM, 0);
        connect(client_fd, (struct sockaddr*)&addr, sizeof(addr));

        // make socket non-blocking
        fcntl(client_fd, F_SETFL, O_NONBLOCK);

        size_t total_sent = 0;
        while (1) {
            ssize_t sent = send(client_fd, chunk, BUF_CHUNK, 0);
            if (sent > 0) {
                total_sent += sent;
            } else if (sent == -1 && errno == EAGAIN) {
                printf("Socket send buffer filled at: %zu bytes\n", total_sent);
                break;
            } else {
                perror("send");
                break;
            }
        }

        close(client_fd);
        free(chunk);
        exit(0);
    }

    // parent = server
    conn_fd = accept(server_fd, NULL, NULL);

    // Don’t read anything — let the client fill the send buffer
    sleep(5);

    close(conn_fd);
    close(server_fd);
    free(chunk);

    return 0;
}

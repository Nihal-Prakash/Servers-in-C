//256MB version

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TEST_PORT 5000
#define BUF_SIZE (256*1024*1024) // 256 MB

int main() {
    int server_fd, client_fd, conn_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    char *buf = malloc(BUF_SIZE);
    if (!buf) { perror("malloc"); return 1; }
    memset(buf, 'A', BUF_SIZE);

    // ----- server -----
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(TEST_PORT);
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 1);

    if (fork() == 0) {
        // child acts as client
        sleep(1);
        client_fd = socket(AF_INET, SOCK_STREAM, 0);
        connect(client_fd, (struct sockaddr*)&addr, sizeof(addr));
        ssize_t sent = send(client_fd, buf, BUF_SIZE, 0);
        printf("send() returned: %zd bytes\n", sent);
        close(client_fd);
        free(buf);
        exit(0);
    }

    // parent acts as server
    conn_fd = accept(server_fd, NULL, NULL);
    char tmp[4096];
    while (recv(conn_fd, tmp, sizeof(tmp), 0) > 0);
    close(conn_fd);
    close(server_fd);
    free(buf);

    return 0;
}

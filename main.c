// main.c — choose mode: server | client
// Usage:
//   Project.exe server
//   Project.exe client

#include <stdio.h>
#include <string.h>

int run_server(void); // from server.c
int run_client(void); // from client.c

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        if (strcmp(argv[1], "server") == 0) return run_server();
        if (strcmp(argv[1], "client") == 0) return run_client();
    }
    printf("Usage:\n  %s server\n  %s client\n", argv[0], argv[0]);
    return 1;
}

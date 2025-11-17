#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define EXIT_SUCCESS 0
#define EXIT_FAIL -127
#define PORT 5665
#define HOST "127.0.0.1"
#define CONNECT_MSG_FMT "[INFO] Connected to %s:%i!\n"

typedef struct {
  char msg[256];
  bool final;
} sock_recv_data_t;

void recv_struct(int fd, sock_recv_data_t *buf);

int main(void) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd <= 0) {
    printf("Failed to create socket!\n");
    exit(EXIT_FAIL);
  }

  struct sockaddr_in addr;
  unsigned int addr_size = sizeof(addr);
  memset(&addr, 0, addr_size);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = inet_pton(AF_INET, HOST, &addr.sin_addr);

  if (inet_pton(AF_INET, HOST, &addr.sin_addr) <= 0) {
    printf("Cannot connect to localhost!\n");
    exit(EXIT_FAIL);
  }

  if (connect(fd, (struct sockaddr *)&addr, addr_size)) {
    printf("Error connecting to socket\n");
    exit(EXIT_FAIL);
  }
  char host[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(addr.sin_addr), host, INET_ADDRSTRLEN);
  unsigned int port = ntohs(addr.sin_port);
  printf(CONNECT_MSG_FMT, host, port);
  char *msg = "Client msg!";
  sock_recv_data_t buffer;
  size_t buffer_size = sizeof(buffer);
  send(fd, msg, strlen(msg), 0);
  recv_struct(fd, &buffer);
  while (true) {
    printf("Recieved: '%s'\n", buffer.msg);
    if (buffer.final)
      break;
    recv_struct(fd, &buffer);
  }
  return EXIT_SUCCESS;
}

void recv_struct(int fd, sock_recv_data_t *buf) {
  size_t total = 0;
  while (total < sizeof(sock_recv_data_t)) {
    ssize_t n =
        recv(fd, ((char *)buf) + total, sizeof(sock_recv_data_t) - total, 0);
    if (n <= 0)
      exit(EXIT_FAIL);
    total += n;
  }
}

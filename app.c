#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <threads.h>

#define PORT 5665
#define HOST "0.0.0.0"
#define EXIT_FAIL -127
#define EXIT_SUCCESS 0
#define PROCESS_TIME 3
#define SEND_BUF "Server accepted this request!"
#define WAIT_MSG_BUF_FMT "Response will take %i seconds!"

typedef struct {
  char msg[256];
  bool final;
} sock_thrd_send_data_t;

typedef struct {
  int port;
  char *host;
  int fd;
} sock_thrd_args_t;

int handle_client(void *arg) {
  sock_thrd_args_t *args = (sock_thrd_args_t *)arg;
  printf("[INFO] Handling client %s:%i...\n", args->host, (*args).port);
  char buf[30];
  sprintf(buf, WAIT_MSG_BUF_FMT, PROCESS_TIME);
  sock_thrd_send_data_t data;
  memset(&data, 0, sizeof(data));
  strncpy(data.msg, buf, sizeof(data.msg) - 1);
  data.msg[sizeof(data.msg) - 1] = '\0';
  data.final = false;
  send(args->fd, &data, sizeof(data), 0);

  sleep(3);

  fflush(stdout);
  sock_thrd_send_data_t final_data;
  memset(&final_data, 0, sizeof(final_data));
  final_data.final = true;
  strncpy(final_data.msg, SEND_BUF, sizeof(final_data.msg) - 1);
  final_data.msg[sizeof(final_data.msg) - 1] = '\0';

  send(args->fd, &final_data, sizeof(final_data), 0);
  free(args->host);
  free(args);
  printf("[INFO] Successfully handled client!\n");
  return 0;
}

int main(void) {

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  unsigned int addr_size = sizeof(addr);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = INADDR_ANY;
  int sock_desc;
  sock_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_desc <= 0) {
    printf("Error creating socket!\n");
    exit(EXIT_FAIL);
  }

  if (bind(sock_desc, (struct sockaddr *)&addr, addr_size) < 0) {
    printf("Failed to bind socket!\n");
    close(sock_desc);
    exit(EXIT_FAIL);
  }
  printf("Socket successfully binded to %s:%i\n", HOST, PORT);
  listen(sock_desc, 5);
  while (true) {
    struct sockaddr_in client_addr;
    unsigned int client_addr_size = sizeof(client_addr);
    int client_fd =
        accept(sock_desc, (struct sockaddr *)&client_addr, &addr_size);
    if (client_fd < 0) {
      printf("Failed to accept connection!\n");
      continue;
    }
    char host[INET_ADDRSTRLEN];
    int port = ntohs(client_addr.sin_port);
    inet_ntop(AF_INET, &client_addr.sin_addr, host, INET_ADDRSTRLEN);
    thrd_t thread;
    sock_thrd_args_t *args = malloc(sizeof(sock_thrd_args_t));
    args->host = strdup(host);
    args->port = port;
    args->fd = client_fd;
    if (thrd_create(&thread, handle_client, args)) {
      printf("Failed to create thread!\n");
      free(args->host);
      free(args);
      exit(EXIT_FAIL);
    }
    thrd_detach(thread);
  }
  printf("Closing socket...\n");
  close(sock_desc);
  return EXIT_SUCCESS;
};

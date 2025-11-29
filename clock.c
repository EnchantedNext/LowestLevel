#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define EXIT_SUCCESS 0
#define EXIT_FAIL -127

volatile sig_atomic_t stop = 0;

void sig_handler(int sig) { stop = 1; }

void gt(char *buf, size_t size) {
  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buf, size, "%H:%M", timeinfo);
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    printf("Usage - %s! No arguments expected.", argv[0]);
    return EXIT_FAIL;
  }

  signal(SIGINT, sig_handler);

  char buffer[80];
  char previous[80];

  gt(buffer, sizeof(buffer));
  strcpy(previous, buffer);

  printf("%s", buffer);
  fflush(stdout);

  do {
    sleep(1);
    gt(buffer, sizeof(buffer));

    if (strcmp(previous, buffer) == 0) {
      strcpy(previous, buffer);
      continue;
    }

    strcpy(previous, buffer);
    printf("\r%s", buffer);
    fflush(stdout);
  } while (!stop);

  printf("\n");
  return EXIT_SUCCESS;
}

#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define E 2.71828
#define DIVIDER 1000

union Val {
  float f;
  int i;
  void *ptr;
};

double getDouble() {
  const pid_t pid = getpid();
  const double pi = M_PI;
  return (pid - pi) / DIVIDER;
}

int main(void) {
  union Val val;
  val.f = E;
  printf("%f\n", val.f);
  val.i = 9;
  printf("%d\n", val.i);
  double d = getDouble();
  val.ptr = &d;
  printf("%p\n", val.ptr);
  printf("%.03f\n", *(double *)val.ptr);
}

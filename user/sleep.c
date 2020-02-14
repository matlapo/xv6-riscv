#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("sleep only accepts a single integer argument\n");
    exit(0);
  }

  int val_int = atoi(argv[1]);

  sleep(val_int);

  exit(0);
}

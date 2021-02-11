#include <stdio.h>

int main() {
  int x = 10;
  while (x--) {
    printf("%d", x);
  }

  putchar('\n');

  {
    for (x = 10; x--; x--) {
      printf("%d", x);
    }
  }

  for (;;) {
    {
      x = 10;
      break;
    }
  }

  printf("%d", x);
  putchar('\n');

  while (1) {
    {
      while (1) {
        break;
      }
      x = 5;
      break;
    }
  }

  printf("%d", x);
  putchar('\n');

  while (0) {
    {}
  }

  for (; 0;)
    ;

  for (x = 0; x;) {
    continue;
  }

  while (1) {
  oznaka:
    while (1) {
      break;
    }

    x = 5;
    break;
  }

  printf("%d", x);
  putchar('\n');

  return 0;
}

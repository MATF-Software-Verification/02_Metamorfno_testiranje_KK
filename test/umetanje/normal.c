#include <stdio.h>

static int i = 1;

int main() {
  int x = 10;
  while (x--)
    printf("%d", x);
  putchar('\n');

  for (;;) {
    x = 10;
    break;
  }

  do {
  } while (0);

  for (; 0;)
    ;

  return 0;
}

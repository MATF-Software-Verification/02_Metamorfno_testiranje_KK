#include <stdio.h>

int main() {
  int x = 10;
  while (x--)
    printf("%d", x);
  putchar('\n');

  for (;;) {
    x = 10;
    break;
  }

  while (0) {
  }

  for (; 0;)
    ;

  return 0;
}

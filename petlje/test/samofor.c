#include <stdio.h>

int main() {
  int x = 10;
  for (; x--;)
    printf("%d", x);

  for (; x--;) {
    printf("%d", x);
    x += 2;
  }

  putchar('\n');

  {
    for (x = 10; x--; x--)
      printf("%d", x);
  }

  for (;;) {
    x = 10;
    break;
  }

  printf("%d", x);
  putchar('\n');

  for (; 1;) {
    for (; 1;)
      break;
    x = 5;
    break;
  }

  printf("%d", x);
  putchar('\n');

  for (; 0;) {
  }

  for (; 0;)
    ;

  for (x = -5; x; x++)
    continue;

  for (x = 0; x;)
    continue;

  return 0;
}

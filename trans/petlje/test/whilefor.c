#include <stdio.h>

int main() {
  int x = 10;
  while (x--)
    printf("%d", x);

  while (x--) {
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

  while (1) {
    while (1)
      break;
    x = 5;
    break;
  }

  printf("%d", x);
  putchar('\n');

  while (0) {
  }

  for (; 0;)
    ;
  
  for (x = -5; x; x++)
    continue;
  
  for (x = 0; x;)
    continue;

  return 0;
}

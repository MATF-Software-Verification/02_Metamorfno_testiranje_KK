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

  // ime je maskirano u staroj verziji
  int cond = 10;
  {
    int cond0 = 1;
    for (; cond0; cond0 = cond > 5)
      cond--;
  }

  // ime je maskirano u staroj verziji;
  // posledica je beskonacna petlja
  for (int i = 0; i < 3; i++) {
    double i = 3.1415926535897931;
    continue;
  }

  return 0;
}

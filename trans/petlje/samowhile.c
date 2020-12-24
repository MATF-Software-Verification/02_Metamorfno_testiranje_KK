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
    {
      x = 10;
      while (x--) {
        printf("%d", x);
        x--;
      }
    }
  }

  while (1) {
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

  while (0)
    ;

  {
    x = -5;
    while (x) {
      {
        x++;
        continue;
      }
      x++;
    }
  }

  {
    x = 0;
    while (x)
      continue;
  }

  // ime je maskirano u staroj verziji
  int cond = 10;
  {
    int cond0 = 1;
    while (cond0) {
      cond--;
      cond0 = cond > 5;
    }
  }

  // ime je maskirano u staroj verziji;
  // posledica je beskonacna petlja
  {
    int i = 0;
    while (i < 3) {
      {
        double i0 = 3.1415926535897931;
        {
          i++;
          continue;
        }
      }
      i++;
    }
  }

  return 0;
}

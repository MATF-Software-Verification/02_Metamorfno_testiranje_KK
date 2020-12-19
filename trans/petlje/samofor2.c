#include <stdio.h>

int main() {
  int x = 10;
  if (x--) {
    int cond = 1;
    for (; cond; cond = x--)
      printf("%d", x);
  }

  if (x--) {
    int cond0 = 1;
    for (; cond0; cond0 = x--) {
      printf("%d", x);
      x += 2;
    }
  }

  putchar('\n');

  {
    {
      x = 10;
      if (x--) {
        int cond1 = 1;
        for (; cond1; cond1 = x--) {
          printf("%d", x);
          x--;
        }
      }
    }
  }

  if (1) {
    int cond2 = 1;
    for (; cond2; cond2 = 1) {
      x = 10;
      break;
    }
  }

  printf("%d", x);
  putchar('\n');

  if (1) {
    int cond3 = 1;
    for (; cond3; cond3 = 1) {
      if (1) {
        int cond8 = 1;
        for (; cond8; cond8 = 1)
          break;
      }

      x = 5;
      break;
    }
  }

  printf("%d", x);
  putchar('\n');

  if (0) {
    int cond4 = 1;
    for (; cond4; cond4 = 0) {
    }
  }

  if (0) {
    int cond5 = 1;
    for (; cond5; cond5 = 0)
      ;
  }

  {
    x = -5;
    if (x) {
      int cond6 = 1;
      for (; cond6; cond6 = x) {
        {
          x++;
          continue;
        }
        x++;
      }
    }
  }

  {
    x = 0;
    if (x) {
      int cond7 = 1;
      for (; cond7; cond7 = x)
        continue;
    }
  }

  return 0;
}

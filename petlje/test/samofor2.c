#include <stdio.h>

int main() {
  int x = 10;
  if (x--) {
    int cond = 1;
    for (; cond; cond = x--)
      printf("%d", x);
  }

  if (x--) {
    int cond = 1;
    for (; cond; cond = x--) {
      printf("%d", x);
      x += 2;
    }
  }

  putchar('\n');

  {
    {
      x = 10;
      if (x--) {
        int cond = 1;
        for (; cond; cond = x--) {
          printf("%d", x);
          x--;
        }
      }
    }
  }

  if (1) {
    int cond = 1;
    for (; cond; cond = 1) {
      x = 10;
      break;
    }
  }

  printf("%d", x);
  putchar('\n');

  if (1) {
    int cond = 1;
    for (; cond; cond = 1) {
      if (1) {
        int cond = 1;
        for (; cond; cond = 1)
          break;
      }
      x = 5;
      break;
    }
  }

  printf("%d", x);
  putchar('\n');

  if (0) {
    int cond = 1;
    for (; cond; cond = 0) {
    }
  }

  if (0) {
    int cond = 1;
    for (; cond; cond = 0)
      ;
  }

  {
    x = -5;
    if (x) {
      int cond = 1;
      for (; cond; cond = x) {
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
      int cond = 1;
      for (; cond; cond = x)
        continue;
    }
  }

  return 0;
}

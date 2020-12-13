#include <stdio.h>

int main() {
  int x = 10;
  if (x--) {
    int cond = 1;
    while (cond) {
      printf("%d", x);
      cond = x--;
    }
  }

  if (x--) {
    int cond = 1;
    while (cond) {
      {
        printf("%d", x);
        x += 2;
      }
      cond = x--;
    }
  }

  putchar('\n');

  {
    {
      x = 10;
      if (x--) {
        int cond = 1;
        while (cond) {
          {
            printf("%d", x);
            x--;
          }
          cond = x--;
        }
      }
    }
  }

  if (1) {
    int cond = 1;
    while (cond) {
      {
        x = 10;
        break;
      }
      cond = 1;
    }
  }

  printf("%d", x);
  putchar('\n');

  if (1) {
    int cond = 1;
    while (cond) {
      {
        if (1) {
          int cond = 1;
          while (cond) {
            break;
            cond = 1;
          }
        }
        x = 5;
        break;
      }
      cond = 1;
    }
  }

  printf("%d", x);
  putchar('\n');

  if (0) {
    int cond = 1;
    while (cond) {
      {
      }
      cond = 0;
    }
  }

  if (0) {
    int cond = 1;
    while (cond) {
      ;
      cond = 0;
    }
  }

  {
    x = -5;
    if (x) {
      int cond = 1;
      while (cond) {
        {
          {
            x++;
            {
              cond = x;
              continue;
            }
          }
          x++;
        }
        cond = x;
      }
    }
  }

  {
    x = 0;
    if (x) {
      int cond = 1;
      while (cond) {
        {
          cond = x;
          continue;
        }
        cond = x;
      }
    }
  }

  return 0;
}

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
    int cond0 = 1;
    while (cond0) {
      {
        printf("%d", x);
        x += 2;
      }
      cond0 = x--;
    }
  }

  putchar('\n');

  {
    {
      x = 10;
      if (x--) {
        int cond1 = 1;
        while (cond1) {
          {
            printf("%d", x);
            x--;
          }
          cond1 = x--;
        }
      }
    }
  }

  if (1) {
    int cond2 = 1;
    while (cond2) {
      {
        x = 10;
        break;
      }
      cond2 = 1;
    }
  }

  printf("%d", x);
  putchar('\n');

  if (1) {
    int cond3 = 1;
    while (cond3) {
      {
        if (1) {
          int cond8 = 1;
          while (cond8) {
            break;
            cond8 = 1;
          }
        }
        x = 5;
        break;
      }
      cond3 = 1;
    }
  }

  printf("%d", x);
  putchar('\n');

  if (0) {
    int cond4 = 1;
    while (cond4) {
      {}
      cond4 = 0;
    }
  }

  if (0) {
    int cond5 = 1;
    while (cond5) {
      ;
      cond5 = 0;
    }
  }

  {
    x = -5;
    if (x) {
      int cond6 = 1;
      while (cond6) {
        {
          {
            x++;
            {
              cond6 = x;
              continue;
            }
          }
          x++;
        }
        cond6 = x;
      }
    }
  }

  {
    x = 0;
    if (x) {
      int cond7 = 1;
      while (cond7) {
        {
          cond7 = x;
          continue;
        }
        cond7 = x;
      }
    }
  }

  return 0;
}

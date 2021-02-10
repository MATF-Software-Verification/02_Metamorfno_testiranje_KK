#include <stdio.h>

int main() {
  int x = 10;
  if (x--) {
    int cond0 = 1;
    while (cond0) {
      printf("%d", x);
      cond0 = x--;
    }
  }

  if (x--) {
    int cond1 = 1;
    while (cond1) {
      {
        printf("%d", x);
        x += 2;
      }
      cond1 = x--;
    }
  }

  putchar('\n');

  {
    {
      x = 10;
      if (x--) {
        int cond2 = 1;
        while (cond2) {
          {
            printf("%d", x);
            x--;
          }
          cond2 = x--;
        }
      }
    }
  }

  if (1) {
    int cond3 = 1;
    while (cond3) {
      {
        x = 10;
        break;
      }
      cond3 = 1;
    }
  }

  printf("%d", x);
  putchar('\n');

  if (1) {
    int cond4 = 1;
    while (cond4) {
      {
        if (1) {
          int cond11 = 1;
          while (cond11) {
            break;
            cond11 = 1;
          }
        }
        x = 5;
        break;
      }
      cond4 = 1;
    }
  }

  printf("%d", x);
  putchar('\n');

  if (0) {
    int cond5 = 1;
    while (cond5) {
      {}
      cond5 = 0;
    }
  }

  if (0) {
    int cond6 = 1;
    while (cond6) {
      ;
      cond6 = 0;
    }
  }

  {
    x = -5;
    if (x) {
      int cond7 = 1;
      while (cond7) {
        {
          {
            x++;
            {
              cond7 = x;
              continue;
            }
          }
          x++;
        }
        cond7 = x;
      }
    }
  }

  {
    x = 0;
    if (x) {
      int cond8 = 1;
      while (cond8) {
        {
          cond8 = x;
          continue;
        }
        cond8 = x;
      }
    }
  }

  // ime je maskirano u staroj verziji
  int cond = 10;
  {
    int cond9 = 1;
    while (cond9) {
      cond--;
      cond9 = cond > 5;
    }
  }

  // ime je maskirano u staroj verziji;
  // posledica je beskonacna petlja
  {
    int i = 0;
    if (i < 3) {
      int cond10 = 1;
      while (cond10) {
        {
          {
            double i0 = 3.1415926535897931;
            {
              i++;
              {
                cond10 = i < 3;
                continue;
              }
            }
          }
          i++;
        }
        cond10 = i < 3;
      }
    }
  }

  return 0;
}

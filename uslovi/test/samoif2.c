#include <stdio.h>

int main() {
  double pi = 3;
  do {
    {
      int br = 0;
      {
        int cond = (int)!!pi;
        do {
          if (cond == 1) {
            {
              putchar((int)pi + '0');
              pi--;
            }
            break;
          }
          {
            br = 1;
            break;
          }
        } while (0);
      }

      if (br)
        break;
    }
  } while (1);

  putchar('\n');

  {
    int cond0 = (int)(1 < 2);
    do {
      if (cond0 == 1)
        putchar('x');
    } while (0);
  }

  int i = 1, j = 2;
  {
    int cond1 = (int)(i > j);
    do {
      if (cond1 == 1) {
        return 1;
        break;
      }
      {
        int cond3 = 0;
        do {
          if (cond3 == 1) {
            { return 1; }
            break;
          }
          { putchar('k'); }
        } while (0);
      }

    } while (0);
  }

  putchar('\n');

  for (int k = 1; k < 6; k++) {
    int cont = 0;
    {
      int cond2 = k;
      do {
        if (cond2 == 1)
          putchar('1');
        if (cond2 == 1 || cond2 == 2) {
          {
            putchar('2');
            putchar('\n');
          }
          break;
        }
        if (cond2 != 3 && cond2 != 4) {
          int cond4 = k - 1;
          do {
            if (cond4 == 4)
              putchar('5');
          } while (0);
        }

        if (cond2 != 4) {
          {
            putchar('3');
            putchar('\n');
          }
          {
            cont = 1;
            break;
          }
        }
        putchar('4');
        putchar('\n');
      } while (0);
    }

    if (cont)
      continue;
  }

  return 0;
}

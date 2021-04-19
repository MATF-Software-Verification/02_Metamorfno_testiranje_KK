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
            {
              br = 1;
              break;
            }
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
      if (cond0 == 1) {
        putchar('x');
      }
    } while (0);
  }

  int i = 1, j = 2;
  if (i > j) {
    return 1;
  } else {
    {
      int cond1 = 0;
      do {
        if (cond1 == 1) {
          { return 1; }
          break;
        }
        { putchar('k'); }
      } while (0);
    }
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
        if (cond2 != 3 && cond2 != 4 && cond2 != 6) {
          int cond7 = k - 1;
          do {
            if (cond7 == 4)
              putchar('5');
          } while (0);
        }

        if (cond2 != 4 && cond2 != 6) {
          {
            putchar('3');
            putchar('\n');
          }
          {
            cont = 1;
            break;
          }
        }
        if (cond2 != 6) {
          {
            putchar('4');
            putchar('\n');
          }
          {
            cont = 1;
            break;
          }
        }
      } while (0);
    }

    if (cont)
      continue;
  }

  // spoljasnji sa dubokim oznakama
  switch (2) {
    while (0) {
    case 1:
      break;
    case 2:
      break;
    case 3:
      continue;
    case 4:
      continue;

      {
        int cond3 = 10;
        do {
          if (cond3 == 10)
            return 0;
          return 1;
        } while (0);
      }
    }
  }

  while (0) {
    continue;
    break;
  }

  do {
    int cont0 = 0;
    {
      int cond4 = 1;
      do {
        {
          cont0 = 1;
          break;
        }
        break;
      } while (0);
    }

    if (cont0)
      continue;
  } while (0);

  do {
    int cont1 = 0;
    {
      int cond5 = 1;
      do {
        if (cond5 != 1) {
          {
            cont1 = 1;
            break;
          }
          break;
        }
        return 1;
      } while (0);
    }

    if (cont1)
      continue;
  } while (0);

  for (; 0;) {
    {
      int br0 = 0;
      {
        int cont2 = 0;
        {
          int cond6 = 1;
          do {
            if (cond6 == 1) {
              {
                br0 = 1;
                break;
              }
              {
                cont2 = 1;
                break;
              }
              {
                br0 = 1;
                break;
              }
            }
          } while (0);
        }

        if (cont2)
          continue;
      }

      if (br0)
        break;
    }
  }

  return 0;
}

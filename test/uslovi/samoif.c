#include <stdio.h>

int main() {
  double pi = 3;
  do {
    if (pi) {
      putchar((int)pi + '0');
      pi--;
    } else
      break;
  } while (1);

  putchar('\n');

  if (1 < 2)
    putchar('x');

  int i = 1, j = 2;
  if (i > j)
    return 1;
  else if (0) {
    return 1;
  } else {
    putchar('k');
  }

  putchar('\n');

  for (int k = 1; k < 6; k++) {
    int cont = 0;
    {
      int cond = k;
      do {
        if (cond == 1)
          putchar('1');
        if (cond == 1 || cond == 2) {
          {
            putchar('2');
            putchar('\n');
          }
          break;
        }
        if (cond != 3 && cond != 4) {
          int cond1 = k - 1;
          do {
            if (cond1 == 4)
              putchar('5');
          } while (0);
        }

        if (cond != 4) {
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

  // spoljasnji sa dubokim oznakama
  switch (2) {
    while (0) {
    case 1:
      break;
    case 2:
      break;

      {
        int cond0 = 10;
        do {
          if (cond0 == 10)
            return 0;
          return 1;
        } while (0);
      }
    }
  }

  return 0;
}

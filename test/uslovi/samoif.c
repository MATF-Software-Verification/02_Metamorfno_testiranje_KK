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
        if (cond != 3 && cond != 4 && cond != 6) {
          int cond3 = k - 1;
          do {
            if (cond3 == 4)
              putchar('5');
          } while (0);
        }

        if (cond != 4 && cond != 6) {
          {
            putchar('3');
            putchar('\n');
          }
          {
            cont = 1;
            break;
          }
        }
        if (cond != 6) {
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
  while (0) {
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
          int cond0 = 10;
          do {
            if (cond0 == 10)
              return 0;
            return 1;
          } while (0);
        }
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
      int cond1 = 1;
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
      int cond2 = 1;
      do {
        if (cond2 != 1) {
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
    if (1) {
      break;
      continue;
      break;
    }
  }

  do {
    if (0) {
      break;
      if (0)
        return 1;
    }
  } while (0);

  do {
    if (0)
      break;
    else if (0)
      return 1;
    else
      continue;
  } while (0);

  return 0;
}

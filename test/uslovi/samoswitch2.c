#include <stdio.h>

int main() {
  double pi = 3;
  do {
    {
      int br = 0;
      switch ((int)!!pi) {
      case 1: {
        putchar((int)pi + '0');
        pi--;
      } break;
      default: {
        br = 1;
        break;
      }
      }

      if (br)
        break;
    }
  } while (1);

  putchar('\n');

  switch ((int)(1 < 2)) {
  case 1:
    putchar('x');
  }

  int i = 1, j = 2;
  switch ((int)(i > j)) {
  case 1:
    return 1;
    break;
  default:
    switch (0) {
    case 1: {
      return 1;
    } break;
    default: {
      putchar('k');
    }
    }
  }

  putchar('\n');

  for (int k = 1; k < 6; k++) {
    int cont = 0;
    {
      int cond = k;
      do {
        switch ((int)(cond == 1)) {
        case 1:
          putchar('1');
        }

        {
          int br0 = 0;
          switch ((int)(cond == 1 || cond == 2)) {
          case 1: {
            {
              putchar('2');
              putchar('\n');
            }
            {
              br0 = 1;
              break;
            }
          }
          }

          if (br0)
            break;
        }
        switch ((int)(cond != 3 && cond != 4)) {
        case 1: {
          int cond1 = k - 1;
          do {
            switch ((int)(cond1 == 4)) {
            case 1:
              putchar('5');
            }

          } while (0);
        }
        }

        {
          int br1 = 0;
          switch ((int)(cond != 4)) {
          case 1: {
            {
              putchar('3');
              putchar('\n');
            }
            {
              cont = 1;
              {
                br1 = 1;
                break;
              }
            }
          }
          }

          if (br1)
            break;
        }
        putchar('4');
        putchar('\n');
      } while (0);
    }

    switch (cont) {
    case 1:
      continue;
    }
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
          switch ((int)(cond0 == 10)) {
          case 1:
            return 0;
          }

          return 1;
        } while (0);
      }
    }
  }

  return 0;
}

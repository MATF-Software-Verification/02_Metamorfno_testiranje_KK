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
        {
          br = 1;
          break;
        }
      }
      }

      if (br)
        break;
    }
  } while (1);

  putchar('\n');

  switch ((int)(1 < 2)) {
  case 1: {
    putchar('x');
  }
  }

  int i = 1, j = 2;
  if (i > j) {
    return 1;
  } else {
    switch ((int)!!0) {
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
        case 1: {
          putchar('1');
        }
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
        if (cond != 3 && cond != 4 && cond != 6) {
          int cond3 = k - 1;
          do {
            switch ((int)(cond3 == 4)) {
            case 1: {
              putchar('5');
            }
            }

          } while (0);
        }

        {
          int br1 = 0;
          switch ((int)(cond != 4 && cond != 6)) {
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
        {
          int br2 = 0;
          switch ((int)(cond != 6)) {
          case 1: {
            {
              putchar('4');
              putchar('\n');
            }
            {
              cont = 1;
              {
                br2 = 1;
                break;
              }
            }
          }
          }

          if (br2)
            break;
        }
      } while (0);
    }

    switch ((int)!!cont) {
    case 1: {
      continue;
    }
    }
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
        int cond0 = 10;
        do {
          switch ((int)(cond0 == 10)) {
          case 1: {
            return 0;
          }
          }

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
      int cond1 = 1;
      do {
        {
          cont0 = 1;
          break;
        }
        break;
      } while (0);
    }

    switch ((int)!!cont0) {
    case 1: {
      continue;
    }
    }

  } while (0);

  do {
    int cont1 = 0;
    {
      int cond2 = 1;
      do {
        {
          int br3 = 0;
          switch ((int)(cond2 != 1)) {
          case 1: {
            {
              cont1 = 1;
              {
                br3 = 1;
                break;
              }
            }
            {
              br3 = 1;
              break;
            }
          }
          }

          if (br3)
            break;
        }
        return 1;
      } while (0);
    }

    switch ((int)!!cont1) {
    case 1: {
      continue;
    }
    }

  } while (0);

  for (; 0;) {
    {
      int br4 = 0;
      switch ((int)!!1) {
      case 1: {
        {
          br4 = 1;
          break;
        }
        continue;
        {
          br4 = 1;
          break;
        }
      }
      }

      if (br4)
        break;
    }
  }

  return 0;
}

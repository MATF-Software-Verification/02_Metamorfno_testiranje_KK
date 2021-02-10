#include <stdio.h>

int main() {
  int x = 10;
  if (x--) {
    int cond0 = 1;
    for (; cond0; cond0 = x--)
      printf("%d", x);
  }

  if (x--) {
    int cond1 = 1;
    for (; cond1; cond1 = x--) {
      printf("%d", x);
      x += 2;
    }
  }

  putchar('\n');

  {
    {
      x = 10;
      if (x--) {
        int cond2 = 1;
        for (; cond2; cond2 = x--) {
          printf("%d", x);
          x--;
        }
      }
    }
  }

  if (1) {
    int cond3 = 1;
    for (; cond3; cond3 = 1) {
      x = 10;
      break;
    }
  }

  printf("%d", x);
  putchar('\n');

  if (1) {
    int cond4 = 1;
    for (; cond4; cond4 = 1) {
      if (1) {
        int cond11 = 1;
        for (; cond11; cond11 = 1)
          break;
      }

      x = 5;
      break;
    }
  }

  printf("%d", x);
  putchar('\n');

  if (0) {
    int cond5 = 1;
    for (; cond5; cond5 = 0) {
    }
  }

  if (0) {
    int cond6 = 1;
    for (; cond6; cond6 = 0)
      ;
  }

  {
    x = -5;
    if (x) {
      int cond7 = 1;
      for (; cond7; cond7 = x) {
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
      int cond8 = 1;
      for (; cond8; cond8 = x)
        continue;
    }
  }

  // ime je maskirano u staroj verziji
  int cond = 10;
  {
    int cond9 = 1;
    for (; cond9; cond9 = cond > 5)
      cond--;
  }

  // ime je maskirano u staroj verziji;
  // posledica je beskonacna petlja
  {
    int i = 0;
    if (i < 3) {
      int cond10 = 1;
      for (; cond10; cond10 = i < 3) {
        {
          double i0 = 3.1415926535897931;
          {
            i++;
            continue;
          }
        }
        i++;
      }
    }
  }

  return 0;
}

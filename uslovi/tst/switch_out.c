#include <stdio.h>

int main() {
  int k;
  do {
    scanf("%d", &k);
    {
      int cont = 0;
      {
        int cond = k;
        do {
          if (cond == 1)
            return 1;
          if (cond == 1 || cond == 2) {
            return 2;
            break;
          }
          if (cond != 3)
            break;
          if (cond == 1 || cond == 2 || cond == 3) {
            return 3;
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
  } while (1);
  return 0;
}

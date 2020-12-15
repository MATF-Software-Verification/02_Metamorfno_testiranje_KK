#include <stdio.h>

int main() {
  int k;
  do {
    scanf("%d", &k);
    {
      int br = 0;
      if (!k) {
        br = 1;
        break;
      } else
        continue;
      if (br)
        break;
    }
  } while (1);
  return 0;
}

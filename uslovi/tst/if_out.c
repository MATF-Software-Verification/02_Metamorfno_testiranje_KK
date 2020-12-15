#include <stdio.h>

int main() {
  double k;
  do {
    scanf("%lf", &k);
    {
      int br = 0;
      switch ((int)!!k) {
      case 1: {
        br = 1;
        break;
      } break;
      default:
        continue;
      }

      if (br)
        break;
    }
  } while (1);
  return 0;
}

#include <stdio.h>

int main() {
  int k;
  do {
    scanf("%d", &k);
    {
      int br = 0;
      switch (!k) {
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

#include <stdio.h>

int main() {
  int k;
  do {
    scanf("%d", &k);
    switch (k) {
    case 1:
      return 1;

    case 2:
      return 2;
      break;

    default:
      break;

    case 3:
      return 3;
      continue;
    }
  } while (1);
  return 0;
}

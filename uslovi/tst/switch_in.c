#include <stdio.h>

int main() {
  int k;
  do {
    scanf("%d", &k);
    switch (k) {
    case 1:
      return 1;

    case 2:
      return 1;
      break;

    default:
      break;

    case 3:
      return 1;
      continue;
    }
  } while (1);
  return 0;
}

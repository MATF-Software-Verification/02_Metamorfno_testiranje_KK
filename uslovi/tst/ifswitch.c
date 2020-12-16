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

  for (int k = 1; k < 6; k++)
    switch (k) {
    case 1:
      putchar('1');

    case 2:
      putchar('2');
      putchar('\n');
      break;

    default:
      switch (k - 1) {
      case 4:
        putchar('5');
      }

    case 3:
      putchar('3');
      putchar('\n');
      continue;

    case 4:
      putchar('4');
      putchar('\n');
    }

  return 0;
}

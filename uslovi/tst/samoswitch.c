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

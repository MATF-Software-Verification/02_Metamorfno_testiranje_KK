#include <stdio.h>

int main() {
  double k;
  do {
    scanf("%lf", &k);
    if (k)
      break;
    else
      continue;
  } while (1);
  return 0;
}

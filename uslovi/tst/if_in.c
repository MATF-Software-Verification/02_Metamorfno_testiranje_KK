#include <stdio.h>

int main() {
  int k;
  do {
    scanf("%d", &k);
    if (!k)
      break;
    else
      continue;
  } while (1);
  return 0;
}

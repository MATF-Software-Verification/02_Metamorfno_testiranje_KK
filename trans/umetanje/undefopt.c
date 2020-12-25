#include <stdio.h>

void main() {
  int ok;
  sscanf("0", "%d", &ok);
  if (!ok) {
    int x[] = {1, 2, 3, 4, 5};
    int y[] = {2, 4, 6, 8, 10};
    float xp = 9, p;
    int yp = 0, n = 6;

    for (int i = 0; i < n; i++) {
      p = 1;

      for (int j = 0; j < n; j++)
        if (i != j)
          p *= (xp - x[j]) / (x[i] - x[j]);

      yp += p * y[i];
    }

    printf("%d\n", yp);
  }
}

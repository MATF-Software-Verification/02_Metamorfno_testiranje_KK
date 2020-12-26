#include <stdio.h>

int main() {
  int x = 10;
  while (x--) {
    {
      int ok;
      sscanf("0", "%d", &ok);
      if (ok) {
        char *t = "hello";
        t[0] = 'H';
        putchar(t[0]);

        int x = 1;
        int y = x / 0;
        printf("%d\n", y);

        int niz[] = {0, 1, 2, 3};
        int *p = niz + 100;
        printf("%d\n", *p);

        p = 0;
        int c = *p, d;
        printf("%d\n", c);

        int a = 0;
        int b = 0;
        if (&a < &b)
          a = b;

        int i = 0;
        i = i++ + 1;

        niz[i] = i++;
        printf("%d %d\n", ++i, i * i);

        int num = -1;
        unsigned int val = 1 << num;
        printf("%u", val);

        num = 32;
        val = 1 << num;
        printf("%u\n", val);

        num = 64;
        unsigned long long val2 = 1ULL << num;
        printf("%llu\n", val2);

        c = d = 0;
        for (b = 4; b > -30; b--)
          for (; c;)
            for (;;) {
              printf("%d", a > 2147483647 - b);
              if (d)
                break;
            }
        putchar('\n');
      }
    }
    printf("%d", x);
  }
  putchar('\n');

  for (;;) {
    x = 10;
    {
      {
        int ok;
        sscanf("0", "%d", &ok);
        if (ok) {
          int x[] = {1, 2, 3, 4, 5};
          int y[] = {2, 4, 6, 8, 10};
          float xp = 9, p;
          int yp = 0, n;
          scanf("%d", &n);

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
      break;
    }
  }

  while (0) {
  }

  for (; 0;)
    ;

  return 0;
}

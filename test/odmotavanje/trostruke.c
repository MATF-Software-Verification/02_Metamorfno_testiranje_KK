#include <stdio.h>

int main() {
  int x = 10;
  while (x--) {
    printf("%d", x);
    if (!x--)
      break;
    printf("%d", x);
    if (!x--)
      break;
    printf("%d", x);
  }

  putchar('\n');

  {
    for (x = 10; x--; x--) {
      printf("%d", x);
      x--;
      if (!x--)
        break;
      printf("%d", x);
      x--;
      if (!x--)
        break;
      printf("%d", x);
    }
  }

  for (;;) {
    {
      x = 10;
      break;
    }
    {
      x = 10;
      break;
    }
    {
      x = 10;
      break;
    }
  }

  printf("%d", x);
  putchar('\n');

  while (1) {
    {
      while (1) {
        break;
        if (!1)
          break;
        break;
        if (!1)
          break;
        break;
      }
      x = 5;
      break;
    }
    if (!1)
      break;
    {
      while (1) {
        break;
        if (!1)
          break;
        break;
        if (!1)
          break;
        break;
      }
      x = 5;
      break;
    }
    if (!1)
      break;
    {
      while (1) {
        break;
        if (!1)
          break;
        break;
        if (!1)
          break;
        break;
      }
      x = 5;
      break;
    }
  }

  printf("%d", x);
  putchar('\n');

  do {
    {}
    if (!0)
      break;
    {}
    if (!0)
      break;
    {}
  } while (0);

  for (; 0;) {
    if (!0)
      break;
    if (!0)
      break;
  }

  for (x = 0; x;) {
    continue;
    if (!x)
      break;
    continue;
    if (!x)
      break;
    continue;
  }

  while (1) {
  oznaka:
    while (1) {
      break;
      if (!1)
        break;
      break;
      if (!1)
        break;
      break;
    }

    x = 5;
    break;
  }

  printf("%d", x);
  putchar('\n');

  return 0;
}

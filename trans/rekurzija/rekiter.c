#include <stdio.h>

static int broj = 5;

void viter() {
  if (broj) {
    printf("%d", broj);
    broj--;
    return viter();
  }
  putchar('\n');
}

int itenr(long a, int rez) {
  if (a <= 0)
    return rez;
  else
    return itenr(a - 1, rez + 1);
}

// duboki polozaj poziva
int itenr2(long a, int rez) {
  if (a <= 0) {
    rez--;
    {
      rez++;
      { return rez; }
    }
  } else {
    rez--;
    {
      rez++;
      { return itenr2(a - 1, rez + 1); }
    }
  }
}

int itern(int a, int rez) {
  if (a > 0)
    return itern(a - 1, rez + 1);
  else
    return rez;
}

// rekurzivni poziv u "bazi"
int itern2(int a, int rez) {
  if (a > 0)
    return itern2(a - 1, rez + 1);
  else {
    if (!a)
      return itern2(a - 1, rez);
    return rez;
  }
}

int ittnr(int a, int rez) {
  if (a <= 0)
    return rez;
  return ittnr(a - 1, rez + 1);
}

// nerekurzivni poziv u bazi
int ittnr2(int a, int rez) {
  if (a <= 0)
    return rez;

  if (a > 5)
    return ittnr(a - 6, rez + 6);

  return ittnr2(a - 1, rez + 1);
}

int ittrn(int a, int rez) {
  if (a > 0)
    return ittrn(a - 1, rez + 1);
  return rez;
}

// return status istog tipa
int ittrn2(int a, int rez) {
  if (a > 0)
    return a;
  return rez;
}

void hello(int i) {
  while (i--)
    printf("hello\n");
}

int main() {
  viter();
  printf("%d%d", itenr(1, 1), itenr2(2, 2));
  printf("%d%d\n", itern(3, 3), itern2(4, 4));

  int x, y;
  void (*f)(int) = &hello;
  while (x > 2) {
    y++;
    break;

    int z = 3;
    x = y - z;
    continue;

    // rad sa fjama
    f(z);
    hello(z);
    return x + 1;
  }

  // ugnezdjene petlje
  do
    while (1)
      break;
  while (0);

  // labela i goto
  for (; 1;)
  labela:
    break;

  printf("%d%d", ittrn(5, 0), ittrn2(6, 0));
  printf("%d%d\n", ittrn(7, 0), ittrn2(8, 0));

  return 0;
}

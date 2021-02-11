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

void test1() {
  if (0)
    return test1();
  else
    ;
}

void test2() {
  if (1)
    ;
  else
    return test2();
}

void test3() {
  if (1)
    ;
  else
    return test2();
}

int main() {
  viter();
  printf("%d%d", itenr(1, 1), itenr2(2, 2));
  printf("%d%d\n", itern(3, 3), itern2(4, 4));

  int x = 3, y = x;
  while (x > 2) {
    y++;
    putchar(y + '0');
    break;
    putchar('x');
  }

  while (x > 2) {
    int z = 3;
    x = y - z;
    putchar(x + '0');
    continue;
    putchar('y');
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
  
  while (0)
    goto labela;

  // rad sa funkcijama
  void (*f)(int) = &hello;
  while (x < 2) {
    f(x + 5);
    putchar('\n');
    hello(x + 3);
    break;
  }

  // petlja sa maskiranjem
  int k = 10;
  while (!k) {
    double k = 1;
    k--;

    while (0) {
      double k = 1;
      k--;
    }
  }

  printf("%d%d", ittrn(5, 0), ittrn2(6, 0));
  printf("%d%d\n", ittrn(7, 0), ittrn2(8, 0));

  return 0;
}

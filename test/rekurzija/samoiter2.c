#include <stdio.h>

static int broj = 5;

void viter() {
  while (broj) {
    printf("%d", broj);
    broj--;
    continue;
  }
  putchar('\n');
}

int itenr(long a, int rez) {
  while (!(a <= 0)) {
    long a0 = a - 1;
    int rez0 = rez + 1;
    a = a0;
    rez = rez0;
    continue;
  }

  return rez;
}

// duboki polozaj poziva
int itenr2(long a, int rez) {
  while (!(a <= 0)) {
    rez--;
    {
      rez++;
      {
        {
          long a1 = a - 1;
          int rez1 = rez + 1;
          a = a1;
          rez = rez1;
          continue;
        }
      }
    }
  }
  {
    rez--;
    {
      rez++;
      { return rez; }
    }
  }
}

int itern(int a, int rez) {
  while (a > 0) {
    int a2 = a - 1;
    int rez2 = rez + 1;
    a = a2;
    rez = rez2;
    continue;
  }

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
  while (!(a <= 0)) {
    int a3 = a - 1;
    int rez3 = rez + 1;
    a = a3;
    rez = rez3;
    continue;
  }

  return rez;
}

// nerekurzivni poziv u bazi
int ittnr2(int a, int rez) {
  while (!(a <= 0)) {
    if (a > 5)
      return ittnr(a - 6, rez + 6);
    {
      int a4 = a - 1;
      int rez4 = rez + 1;
      a = a4;
      rez = rez4;
      continue;
    }
  }
  return rez;
}

int ittrn(int a, int rez) {
  while (a > 0) {
    int a5 = a - 1;
    int rez5 = rez + 1;
    a = a5;
    rez = rez5;
    continue;
  }

  return rez;
}

// return status istog tipa
int ittrn2(int a, int rez) {
  if (a > 0)
    return a;
  return rez;
}

int petlja(int *i) {
  while ((*i)--) {
    printf("hello\n");
    {
      int *i0 = i;
      i = i0;
      continue;
    }
  }
  return 0;
}

void hello(int i) {
  if (petlja(&i))
    return;
}

void test1() {
  while (0)
    continue;

  ;
}

void test2() {
  while (!1)
    continue;

  ;
}

void test3() {
  if (1)
    ;
  else
    return test2();
}

void test4();

void test4() {
  if (0)
    ;
  return;
}

int petlja0(int *x, int *y, int *ret) {
  while ((*x) > 2) {
    {
      (*y)++;
      putchar((*y) + '0');
      return 0;
      putchar('x');
    }
    {
      int *x0 = x;
      int *y0 = y;
      int *ret0 = ret;
      x = x0;
      y = y0;
      ret = ret0;
      continue;
    }
  }
  return 0;
}

int petlja1(int *x, int *y, int *ret0) {
  while ((*x) > 2) {
    {
      int z = 3;
      (*x) = (*y) - z;
      putchar((*x) + '0');
      {
        int *x1 = x;
        int *y1 = y;
        int *ret00 = ret0;
        x = x1;
        y = y1;
        ret0 = ret00;
        continue;
      }

      putchar('y');
    }
    {
      int *x2 = x;
      int *y2 = y;
      int *ret01 = ret0;
      x = x2;
      y = y2;
      ret0 = ret01;
      continue;
    }
  }
  return 0;
}

int petlja2(int *ret1) {
  while (1) {
    return 0;
    {
      int *ret10 = ret1;
      ret1 = ret10;
      continue;
    }
  }
  return 0;
}

int petlja3(int *cond, int *ret2) {
  while ((*cond)) {
    {
      {
        int ret1;
        if (petlja2(&ret1)) {
          (*ret2) = ret1;
          return 1;
        }
      }
      (*cond) = 0;
    }
    {
      int *cond0 = cond;
      int *ret20 = ret2;
      cond = cond0;
      ret2 = ret20;
      continue;
    }
  }
  return 0;
}

int petlja4(int *x, void (**f)(int), int *ret3) {
  while ((*x) < 2) {
    {
      (*f)((*x) + 5);
      putchar('\n');
      hello((*x) + 3);
      return 0;
    }
    {
      int *x3 = x;
      void (**f0)(int) = f;
      int *ret30 = ret3;
      x = x3;
      f = f0;
      ret3 = ret30;
      continue;
    }
  }
  return 0;
}

int petlja5(int *ret4) {
  while (0) {
    {
      double k = 1;
      k--;
    }
    {
      int *ret40 = ret4;
      ret4 = ret40;
      continue;
    }
  }
  return 0;
}

int main() {
  viter();
  printf("%d%d", itenr(1, 1), itenr2(2, 2));
  printf("%d%d\n", itern(3, 3), itern2(4, 4));

  int x = 3, y = x;
  {
    int ret;
    if (petlja0(&x, &y, &ret))
      return ret;
  }

  {
    int ret0;
    if (petlja1(&x, &y, &ret0))
      return ret0;
  }

  // ugnezdjene petlje
  {
    int cond = 1;
    {
      int ret2;
      if (petlja3(&cond, &ret2))
        return ret2;
    }
  }

  // labela i goto
  while (1)
  labela:
    break;

  while (0)
    goto labela;

  // rad sa funkcijama
  void (*f)(int) = &hello;
  {
    int ret3;
    if (petlja4(&x, &f, &ret3))
      return ret3;
  }

  // petlja sa maskiranjem
  int k = 10;
  while (!k) {
    double k = 1;
    k--;

    {
      int ret4;
      if (petlja5(&ret4))
        return ret4;
    }
  }

  printf("%d%d", ittrn(5, 0), ittrn2(6, 0));
  printf("%d%d\n", ittrn(7, 0), ittrn2(8, 0));

  test1();
  test2();
  test3();
  test4();

  return 0;
}

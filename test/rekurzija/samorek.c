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

int petlja(int *i) {
  if ((*i)--) {
    printf("hello\n");
    return petlja(i);
  }

  return 0;
}

void hello(int i) {
  if (petlja(&i))
    return;
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

void test4();

void test4() {
  if (0)
    ;
  return;
}

int petlja0(int *x, int *y, int *ret) {
  if ((*x) > 2) {
    {
      (*y)++;
      putchar((*y) + '0');
      return 0;
      putchar('x');
    }
    return petlja0(x, y, ret);
  }

  return 0;
}

int petlja1(int *x, int *y, int *ret0) {
  if ((*x) > 2) {
    {
      int z = 3;
      (*x) = (*y) - z;
      putchar((*x) + '0');
      return petlja1(x, y, ret0);
      putchar('y');
    }
    return petlja1(x, y, ret0);
  }

  return 0;
}

int petlja2(int *ret1) {
  if (1) {
    return 0;
    return petlja2(ret1);
  }

  return 0;
}

int petlja3(int *cond, int *ret2) {
  if ((*cond)) {
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
    return petlja3(cond, ret2);
  }

  return 0;
}

int petlja4(int *x, void (**f)(int), int *ret3) {
  if ((*x) < 2) {
    {
      (*f)((*x) + 5);
      putchar('\n');
      hello((*x) + 3);
      return 0;
    }
    return petlja4(x, f, ret3);
  }

  return 0;
}

int petlja5(int *ret4) {
  if (0) {
    {
      double k = 1;
      k--;
    }
    return petlja5(ret4);
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

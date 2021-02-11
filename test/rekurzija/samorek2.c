#include <stdio.h>

static int broj = 5;

int petlja() {
  if (broj) {
    {
      printf("%d", broj);
      broj--;
      return petlja();
    }
    return petlja();
  }

  return 0;
}

void viter() {
  if (petlja())
    return;

  putchar('\n');
}

int petlja0(long *a, int *rez, int *ret) {
  if (!((*a) <= 0)) {
    {
      long a0 = (*a) - 1;
      int rez0 = (*rez) + 1;
      (*a) = a0;
      (*rez) = rez0;
      return petlja0(a, rez, ret);
    }
    return petlja0(a, rez, ret);
  }

  return 0;
}

int itenr(long a, int rez) {
  {
    int ret;
    if (petlja0(&a, &rez, &ret))
      return ret;
  }

  return rez;
}

// duboki polozaj poziva
int petlja1(long *a, int *rez, int *ret0) {
  if (!((*a) <= 0)) {
    {
      (*rez)--;
      {
        (*rez)++;
        {
          {
            long a1 = (*a) - 1;
            int rez1 = (*rez) + 1;
            (*a) = a1;
            (*rez) = rez1;
            return petlja1(a, rez, ret0);
          }
        }
      }
    }
    return petlja1(a, rez, ret0);
  }

  return 0;
}

int itenr2(long a, int rez) {
  {
    int ret0;
    if (petlja1(&a, &rez, &ret0))
      return ret0;
  }

  {
    rez--;
    {
      rez++;
      { return rez; }
    }
  }
}

int petlja2(int *a, int *rez, int *ret1) {
  if ((*a) > 0) {
    {
      int a2 = (*a) - 1;
      int rez2 = (*rez) + 1;
      (*a) = a2;
      (*rez) = rez2;
      return petlja2(a, rez, ret1);
    }
    return petlja2(a, rez, ret1);
  }

  return 0;
}

int itern(int a, int rez) {
  {
    int ret1;
    if (petlja2(&a, &rez, &ret1))
      return ret1;
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

int petlja3(int *a, int *rez, int *ret2) {
  if (!((*a) <= 0)) {
    {
      int a3 = (*a) - 1;
      int rez3 = (*rez) + 1;
      (*a) = a3;
      (*rez) = rez3;
      return petlja3(a, rez, ret2);
    }
    return petlja3(a, rez, ret2);
  }

  return 0;
}

int ittnr(int a, int rez) {
  {
    int ret2;
    if (petlja3(&a, &rez, &ret2))
      return ret2;
  }

  return rez;
}

// nerekurzivni poziv u bazi
int petlja4(int *a, int *rez, int *ret3) {
  if (!((*a) <= 0)) {
    {
      if ((*a) > 5) {
        (*ret3) = ittnr((*a) - 6, (*rez) + 6);
        return 1;
      }
      {
        int a4 = (*a) - 1;
        int rez4 = (*rez) + 1;
        (*a) = a4;
        (*rez) = rez4;
        return petlja4(a, rez, ret3);
      }
    }
    return petlja4(a, rez, ret3);
  }

  return 0;
}

int ittnr2(int a, int rez) {
  {
    int ret3;
    if (petlja4(&a, &rez, &ret3))
      return ret3;
  }

  return rez;
}

int petlja5(int *a, int *rez, int *ret4) {
  if ((*a) > 0) {
    {
      int a5 = (*a) - 1;
      int rez5 = (*rez) + 1;
      (*a) = a5;
      (*rez) = rez5;
      return petlja5(a, rez, ret4);
    }
    return petlja5(a, rez, ret4);
  }

  return 0;
}

int ittrn(int a, int rez) {
  {
    int ret4;
    if (petlja5(&a, &rez, &ret4))
      return ret4;
  }

  return rez;
}

// return status istog tipa
int ittrn2(int a, int rez) {
  if (a > 0)
    return a;
  return rez;
}

int petlja6(int *i) {
  if ((*i)--) {
    printf("hello\n");
    return petlja6(i);
  }

  return 0;
}

void hello(int i) {
  if (petlja6(&i))
    return;
}

int petlja7() {
  if (0) {
    return petlja7();
    return petlja7();
  }

  return 0;
}

void test1() {
  if (petlja7())
    return;

  ;
}

int petlja8() {
  if (!1) {
    return petlja8();
    return petlja8();
  }

  return 0;
}

void test2() {
  if (petlja8())
    return;

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

int petlja9(int *x, int *y, int *ret5) {
  if ((*x) > 2) {
    {
      (*y)++;
      putchar((*y) + '0');
      return 0;
      putchar('x');
    }
    return petlja9(x, y, ret5);
  }

  return 0;
}

int petlja10(int *x, int *y, int *ret6) {
  if ((*x) > 2) {
    {
      int z = 3;
      (*x) = (*y) - z;
      putchar((*x) + '0');
      return petlja10(x, y, ret6);
      putchar('y');
    }
    return petlja10(x, y, ret6);
  }

  return 0;
}

int petlja11(int *ret7) {
  if (1) {
    return 0;
    return petlja11(ret7);
  }

  return 0;
}

int petlja12(int *cond, int *ret8) {
  if ((*cond)) {
    {
      {
        int ret7;
        if (petlja11(&ret7)) {
          (*ret8) = ret7;
          return 1;
        }
      }
      (*cond) = 0;
    }
    return petlja12(cond, ret8);
  }

  return 0;
}

int petlja13(int *x, void (**f)(int), int *ret9) {
  if ((*x) < 2) {
    {
      (*f)((*x) + 5);
      putchar('\n');
      hello((*x) + 3);
      return 0;
    }
    return petlja13(x, f, ret9);
  }

  return 0;
}

int petlja14(int *ret10) {
  if (0) {
    {
      double k = 1;
      k--;
    }
    return petlja14(ret10);
  }

  return 0;
}

int main() {
  viter();
  printf("%d%d", itenr(1, 1), itenr2(2, 2));
  printf("%d%d\n", itern(3, 3), itern2(4, 4));

  int x = 3, y = x;
  {
    int ret5;
    if (petlja9(&x, &y, &ret5))
      return ret5;
  }

  {
    int ret6;
    if (petlja10(&x, &y, &ret6))
      return ret6;
  }

  // ugnezdjene petlje
  {
    int cond = 1;
    {
      int ret8;
      if (petlja12(&cond, &ret8))
        return ret8;
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
    int ret9;
    if (petlja13(&x, &f, &ret9))
      return ret9;
  }

  // petlja sa maskiranjem
  int k = 10;
  while (!k) {
    double k = 1;
    k--;

    {
      int ret10;
      if (petlja14(&ret10))
        return ret10;
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

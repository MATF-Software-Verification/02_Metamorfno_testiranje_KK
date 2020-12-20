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

int petlja0(double *a, int *rez, int *ret) {
  if (!((*a) <= 0)) {
    {
      double a0;
      int rez0 = (*rez) + 1;
      (*rez) = rez0;
      return petlja0(a, rez, ret);
    }
    return petlja0(a, rez, ret);
  }

  return 0;
}

int itenr(double a, int rez) {
  {
    int ret;
    if (petlja0(&a, &rez, &ret))
      return ret;
  }

  return rez;
}

// duboki polozaj poziva
int petlja1(double *a, int *rez, int *ret0) {
  if (!((*a) <= 0)) {
    {
      (*rez)--;
      {
        (*rez)++;
        {
          {
            double a1;
            int rez1 = (*rez) + 1;
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

int itenr2(double a, int rez) {
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

int petlja8(int *ret6) {
  if (1) {
    return 0;
    return petlja8(ret6);
  }

  return 0;
}

int petlja7(int *x, int *y, void (**f)(int), int *ret5) {
  if ((*x) > 2) {
    {
      (*y)++;
      return 0;
      int z = 3;
      (*x) = (*y) - z;
      return petlja7(x, y, f, ret5);
      hello(z);
      {
        (*ret5) = (*x) + 1;
        return 1;
      }
    }
    return petlja7(x, y, f, ret5);
  }

  return 0;
}

int petlja9(int *cond, int *ret7) {
  if ((*cond)) {
    {
      {
        int ret6;
        if (petlja8(&ret6)) {
          (*ret7) = ret6;
          return 1;
        }
      }
      (*cond) = 0;
    }
    return petlja9(cond, ret7);
  }

  return 0;
}

int main() {
  viter();
  printf("%d%d", itenr(1, 1), itenr2(2, 2));
  printf("%d%d\n", itern(3, 3), itern2(4, 4));

  int x, y;
  void (*f)(int) = &hello;
  {
    int ret5;
    if (petlja7(&x, &y, &f, &ret5))
      return ret5;
  }

  // ugnezdjene petlje
  {
    int cond = 1;
    {
      int ret7;
      if (petlja9(&cond, &ret7))
        return ret7;
    }
  }

  // labela i goto
  while (1)
  labela:
    break;

  printf("%d%d", ittrn(5, 0), ittrn2(6, 0));
  printf("%d%d\n", ittrn(7, 0), ittrn2(8, 0));

  return 0;
}

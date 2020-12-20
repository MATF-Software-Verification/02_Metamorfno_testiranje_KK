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

int petlja1(int *ret0) {
  while (1) {
    return 0;
    {
      int *ret00 = ret0;
      ret0 = ret00;
      continue;
    }
  }
  return 0;
}

int petlja0(int *x, int *y, void (**f)(int), int *ret) {
  while ((*x) > 2) {
    {
      (*y)++;
      return 0;
      int z = 3;
      (*x) = (*y) - z;
      {
        int *x0 = x;
        int *y0 = y;
        void (**f0)(int) = f;
        int *ret1 = ret;
        x = x0;
        y = y0;
        f = f0;
        ret = ret1;
        continue;
      }

      (*f)(z);
      hello(z);
      {
        (*ret) = (*x) + 1;
        return 1;
      }
    }
    {
      int *x1 = x;
      int *y1 = y;
      void (**f1)(int) = f;
      int *ret2 = ret;
      x = x1;
      y = y1;
      f = f1;
      ret = ret2;
      continue;
    }
  }
  return 0;
}

int petlja2(int *cond, int *ret1) {
  while ((*cond)) {
    {
      {
        int ret0;
        if (petlja1(&ret0)) {
          (*ret1) = ret0;
          return 1;
        }
      }
      (*cond) = 0;
    }
    {
      int *cond0 = cond;
      int *ret10 = ret1;
      cond = cond0;
      ret1 = ret10;
      continue;
    }
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
    int ret;
    if (petlja0(&x, &y, &f, &ret))
      return ret;
  }

  // ugnezdjene petlje
  {
    int cond = 1;
    {
      int ret1;
      if (petlja2(&cond, &ret1))
        return ret1;
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

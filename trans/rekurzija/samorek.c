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

int itenr(double a, int rez) {
  if (a <= 0)
    return rez;
  else
    return itenr(a - 1, rez + 1);
}

// duboki polozaj poziva
int itenr2(double a, int rez) {
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

int petlja1(int *ret0) {
  if (1) {
    return 0;
    return petlja1(ret0);
  }

  return 0;
}

int petlja0(int *x, int *y, void (**f)(int), int *ret) {
  if ((*x) > 2) {
    {
      (*y)++;
      return 0;
      int z = 3;
      (*x) = (*y) - z;
      return petlja0(x, y, f, ret);
      hello(z);
      {
        (*ret) = (*x) + 1;
        return 1;
      }
    }
    return petlja0(x, y, f, ret);
  }

  return 0;
}

int petlja2(int *cond, int *ret1) {
  if ((*cond)) {
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
    return petlja2(cond, ret1);
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

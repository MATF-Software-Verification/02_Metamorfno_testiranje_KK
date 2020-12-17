#include <stdio.h>

int itenr(int a, int rez) {
  while (!(a <= 0))
    return itenr(a - 1, rez + 1);
  return rez;
}

// duboki polozaj poziva
int itenr2(int a, int rez) {
  while (!(a <= 0)) {
    rez--;
    {
      rez++;
      { return itenr2(a - 1, rez + 1); }
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
  while (a > 0)
    return itern(a - 1, rez + 1);
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
  while (!(a <= 0))
    return ittnr(a - 1, rez + 1);
  return rez;
}

// nerekurzivni poziv u bazi
int ittnr2(int a, int rez) {
  while (!(a <= 0)) {
    if (a > 5)
      return ittnr(a - 6, rez + 6);
    return ittnr2(a - 1, rez + 1);
  }
  return rez;
}

int ittrn(int a, int rez) {
  while (a > 0)
    return ittrn(a - 1, rez + 1);
  return rez;
}

// return status istog tipa
int ittrn2(int a, int rez) {
  if (a > 0)
    return a;
  return rez;
}

int main() {
  printf("%d%d", itenr(1, 1), itenr2(2, 2));
  printf("%d%d\n", itern(3, 3), itern2(4, 4));
  printf("%d%d", ittrn(5, 0), ittrn2(6, 0));
  printf("%d%d\n", ittrn(7, 0), ittrn2(8, 0));
  return 0;
}

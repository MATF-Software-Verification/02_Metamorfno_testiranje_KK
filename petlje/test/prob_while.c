int main() {
  // Ime je zamaskirano;
  // beskonacna petlja
  {
    int i = 0;
    while (i < 3) {
      {
        double i = 3.1415926535897931;
        {
          i++;
          continue;
        }
      }
      i++;
    }
  }

  return 0;
}

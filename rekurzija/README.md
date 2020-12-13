#### clang-rekurzija

## Transformacija rekurzije :repeat:
Program transformiše svu kontrolu toka sa repnim ponavljanjem u C kodu u željeni tip (iteracija, rekurzija), koristeći Кlangov aplikativni programski interfejs prema apstraktnom sintaksnom stablu. U najvećoj meri je održana semantička jednakost polaznog i izmenjenog koda.

## Podešavanje :memo:
Primer prevođenja programa sistemom CMake:
```
mkdir build
cd build
cmake ..
make
```

Primer pokretanja (testiranja) programa:
```
./rekurzija <ulaz> <izlaz> <iter|rek>
./rekurzija ulaz.c izlaz.c iter
```

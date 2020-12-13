#### clang-uslovi

## Transformacija uslova :grey_question:
Program transformiše sve uslove u C kodu u željeni tip (if-then-else, switch-case-default), koristeći Кlangov aplikativni programski interfejs prema apstraktnom sintaksnom stablu. U najvećoj meri je održana semantička jednakost polaznog i izmenjenog koda.

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
./uslovi <ulaz> <izlaz> <if|switch>
./uslovi ulaz.c izlaz.c if
```

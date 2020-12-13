#### clang-petlje

## Transformacija petlji :loop:
Program transformiše sve petlje u C kodu u željeni tip (for, while, do-while), koristeći Кlangov aplikativni programski interfejs prema apstraktnom sintaksnom stablu. U najvećoj meri je održana semantička jednakost polaznog i izmenjenog koda.

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
./petlje <ulaz> <izlaz> <do|while|for>
./petlje ulaz.c izlaz.c do
```


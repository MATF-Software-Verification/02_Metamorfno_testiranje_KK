#### clang-trans

## Transformacija petlji :loop:
Program transformiše sve petlje u C kodu u željeni tip (for, while, do-while), koristeći Кlangov aplikativni programski interfejs prema apstraktnom sintaksnom stablu. U najvećoj meri je održana semantička jednakost polaznog i izmenjenog koda.

Primer pokretanja (testiranja) transformatora:
```
./trans <ulaz> <izlaz> <do|while|for>
./trans ulaz.c izlaz.c do
```

## Transformacija uslova :grey_question:
Program transformiše sve uslove u C kodu u željeni tip (if-then-else, switch-case-default), koristeći Кlangov aplikativni programski interfejs prema apstraktnom sintaksnom stablu. U najvećoj meri je održana semantička jednakost polaznog i izmenjenog koda.

Primer pokretanja (testiranja) programa:
```
./trans <ulaz> <izlaz> <if|switch>
./trans ulaz.c izlaz.c if
```

## Transformacija rekurzije :repeat:
Program transformiše svu kontrolu toka sa repnim ponavljanjem u C kodu u željeni tip (iteracija, rekurzija), koristeći Кlangov aplikativni programski interfejs prema apstraktnom sintaksnom stablu. U najvećoj meri je održana semantička jednakost polaznog i izmenjenog koda.

Primer pokretanja (testiranja) programa:
```
./trans <ulaz> <izlaz> <iter|rek>
./trans ulaz.c izlaz.c iter
```

## Podešavanje :memo:
Primer prevođenja programa sistemom CMake:
```
mkdir build
cd build
cmake ..
make
```


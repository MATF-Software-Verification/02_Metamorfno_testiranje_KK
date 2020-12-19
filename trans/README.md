#### clang-trans

## Transformacija petlji :loop:
Program transformiše sve petlje u C kodu u željeni tip – for, while, do-while.

Primer pokretanja (testiranja) transformatora:
```
./trans <ulaz> <izlaz> <do|while|for>
./trans whilefor.c samodo.c do
```

## Transformacija uslova :grey_question:
Program transformiše sve uslove u C kodu u željeni tip – if-then-else, switch-case-default.

Primer pokretanja (testiranja) programa:
```
./trans <ulaz> <izlaz> <if|switch>
./trans ifswitch.c samoif.c if
```

## Transformacija rekurzije :repeat:
Program transformiše svu kontrolu toka sa repnim ponavljanjem u C kodu u željeni tip – iteracija, rekurzija.

Primer pokretanja (testiranja) programa:
```
./trans <ulaz> <izlaz> <iter|rek>
./trans rekiter.c samoiter.c iter
```

## Podešavanje :memo:
Primer prevođenja programa sistemom CMake:
```
mkdir build
cd build
cmake ..
make
```


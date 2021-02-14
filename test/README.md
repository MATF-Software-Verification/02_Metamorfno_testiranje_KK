#### clang-test

## Testiranje transformatora :mag_right:
Svita testova proverava ispravnost implementiranih transformacija. Transformisani program semantički je ekvivalentan polaznom, što se utvrđuje proverom jednakosti izlaza izvršivih verzija, nastalih prevođenjem bez optimizacija.

Primenjena je tehnika testiranja bele kutije, koja uzima u obzir različite putanje izvršavanja, pri čemu je postignuta faktički potpuna (~100%) pokrivenost linija koda i funkcija, kao i sasvim zadovoljavajuća visoka (91%) pokrivenost grana.

## Podešavanje :memo:
Primer prevođenja svite testova sistemom CMake:
```
mkdir build
cd build
cmake ..
make
```


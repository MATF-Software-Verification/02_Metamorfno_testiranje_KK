#### clang-test

## Testiranje transformatora :mag_right:
Svita testova proverava ispravnost implementiranih transformacija. Transformisani program semantički je ekvivalentan polaznom, što se utvrđuje proverom jednakosti izlaza izvršivih verzija, nastalih prevođenjem bez optimizacija, kako bi se izbegao uticaj eventualnih grešaka u prevodiocu.

Primenjena je tehnika testiranja bele kutije, koje uzima u obiz različite putanje izvršavanja, pri čemu je postignuta faktički potpuna (~100%) pokrivenost linija koda i funkcija, kao i visoka (91%) pokrivenost grana.

## Podešavanje :memo:
Primer prevođenja svite testova sistemom CMake:
```
mkdir build
cd build
cmake ..
make
```


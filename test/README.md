#### clang-test

MTK je aplikacija za metamorfno testiranje kompilatora, po čemu je i dobila naziv. Njen značajan deo čini transformator, čiji je cilj da generiše semantički ekvivalentne programe od nekog ulaznog. Važno je utvrditi ispravnost samog transformatora, što je prodiskutovano u nastavku, a utvrđuje se proverom jednakosti izlaza izvršivih verzija, nastalih prevođenjem bez optimizacija.

Zavisnosti za test svitu, pored onih za sam transformator, jesu g++-10 i libqt5test5. Neophodno je imati prevodilac g++ 10, kako bi se bez problema tumačila pokrivenost koda alatom lcov, kao i biblioteku (Qt5Test5) za testiranje, što su najnovije stabilne verzije u vreme pisanja rada. Za izgradnju se preporučuje upotreba sistema CMake, pomoću dostupne datoteke CMakeLists.txt.

## Testiranje transformatora :mag_right:
Svita testova proverava ispravnost implementiranih transformacija. Transformisani program semantički je ekvivalentan polaznom, što se utvrđuje proverom jednakosti izlaza izvršivih verzija, nastalih prevođenjem bez optimizacija.

U okviru klase MTKTest implementirana je cela svita testova. Redom je testirana svaka grupa transformacija: izmene i odmotavanje petlji, izmene uslova, izmene rekurzije, imputacija koda. Na kraju je proveren rad sa raznim tipovima neispravnog izlaza.

Primenjena je tehnika testiranja bele kutije, koja uzima u obzir različite putanje izvršavanja, pri čemu je postignuta faktički potpuna (~100%) pokrivenost linija koda i funkcija, kao i sasvim zadovoljavajuća visoka (~93%) pokrivenost grana.

## Podešavanje :memo:
Primer prevođenja svite testova sistemom CMake:
```
mkdir build
cd build
cmake ..
make
```


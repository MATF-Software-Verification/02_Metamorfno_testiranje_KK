#### Testiranje kompilatora

## Metamorfno testiranje :loop:
Praktični seminarski rad na kursu [Verifikacija softvera](http://www.verifikacijasoftvera.matf.bg.ac.rs/). Implementiran je algoritam i infrastruktura za generisanje metamorfnih test primera za proveru ispravnosti programskih prevodilaca.

Konstruisano je nekoliko smislenih semantičkih transformacija izvornog koda, koristeći [Clangov](https://clang.llvm.org/) aplikativni programski interfejs prema apstraktnom sintaksnom stablu, za koje je zatim provereno da li se izvršivi kod isto ponaša.

Teorijska podloga o metamorfnom testiranju kompilatora – [autorski](http://www.verifikacijasoftvera.matf.bg.ac.rs/vs/predavanja/02_testiranje/32_LazarVasovic_EMI.pdf) teorijski seminarski rad sa istog kursa.

Polazna tačka za način transformacije petlji – [autorski](https://github.com/matfija/Clang-petlje) seminarski rad sa Konstrukcije kompilatora.

## Podešavanje :memo:
Primer podešavanja zavisnosti za Ubuntu:
```
sudo apt install g++-10 clang-11 libclang-11-dev llvm-11-dev libqt5test5
```
Korišćeni sistem izgradnje je [CMake](https://cmake.org/).

## Članovi tima :boy:
* [Momir Adžemović](https://github.com/Robotmurlock), 1005/2020
* [Lazar Vasović](https://github.com/matfija), 1011/2020
* [Marko Spasić](https://github.com/spaske00), 1014/2020


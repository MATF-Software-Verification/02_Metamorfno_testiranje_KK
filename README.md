#### Testiranje kompilatora

## Metamorfno testiranje :loop:
Praktični seminarski rad na kursu Verifikacija softvera. Implementiran je algoritam i infrastruktura za generisanje metamorfnih test primera za proveru ispravnosti programskih prevodilaca.

Konstruisano je nekoliko smislenih semantičkih transformacija izvornog koda, za koje je zatim provereno da li se izvršivi kod isto ponaša.

Teorijska podloga: [autorski](http://www.verifikacijasoftvera.matf.bg.ac.rs/vs/predavanja/02_testiranje/32_LazarVasovic_EMI.pdf) teorijski seminarski rad sa istog kursa.

Ideja za transformaciju petlji: [autorski](https://github.com/matfija/Clang-petlje) seminarski rad sa Konstrukcije kompilatora.

## Podešavanje :memo:
Primer podešavanja zavisnosti za Ubuntu:
```
sudo apt install libclang-11-dev llvm-11-dev
```
Korišćeni sistem izgradnje je CMake.

## Članovi tima :boy:
* [Momir Adžemović](https://github.com/Robotmurlock), 1005/2020
* [Lazar Vasović](https://github.com/matfija), 1011/2020
* [Marko Spasić](https://github.com/spaske00), 1014/2020


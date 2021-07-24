#### Testiranje kompilatora

## Metamorfno testiranje :loop:
Praktični seminarski rad na kursu [Verifikacija softvera](http://www.verifikacijasoftvera.matf.bg.ac.rs/). Implementiran je algoritam i infrastruktura za generisanje metamorfnih test primera za proveru ispravnosti programskih prevodilaca.

Konstruisano je nekoliko smislenih semantičkih transformacija izvornog koda, koristeći [Clangov](https://clang.llvm.org/) aplikativni programski interfejs prema apstraktnom sintaksnom stablu, za koje je zatim provereno da li se izvršivi kod isto ponaša.

Teorijska podloga o metamorfnom testiranju kompilatora – [autorski](http://www.verifikacijasoftvera.matf.bg.ac.rs/vs/predavanja/02_testiranje/32_LazarVasovic_EMI.pdf) teorijski seminarski rad sa istog kursa.

Polazna tačka za način transformacije petlji – [autorski](https://github.com/matfija/Clang-petlje) seminarski rad sa Konstrukcije kompilatora.

## Podešavanje :memo:
Primer podešavanja zavisnosti za Ubuntu:
```
sudo apt install g++-10 clang-11 libclang-11-dev llvm-11-dev clang-format-11 qtbase5-dev csmith python3.9
```
Korišćeni sistem izgradnje je [CMake](https://cmake.org/).

## Upustvo za koriscenje skripte :neckbeard:
Sve biblioteke su iz standardne `python` biblioteke. Preporučena verzija za `python` je `3.9+`. Pokretanje: 
- `python verify.py`, odnosno:
- `python3 verify.py`

U tom slucaju se skripti pokreće sa podrazumevanim vrednostima za parametre. Primer izlaza:
```
[verify-transformator]: Prevodjenje transformator biblioteke!
...
[verify-global]: Iteracija 1:
[verify-global]: Generisanje C programa...
[csmith-gen]: Seme programa je 5474532256682857071.
[csmith-gen]: Generisan je novi program!
[verify-global]: Transformacija c program...
[verify-global]: Uporedjivanje sumi...
[verify-global]: Test je prosao :)
[verify-global]: Kraj!

[verify-global]: Iteracija 2:
[verify-global]: Generisanje C programa...
[csmith-gen]: Seme programa je 3854015491999104204.
[csmith-gen]: Generisan je novi program!
[verify-global]: Transformacija c program...
[verify-global]: Uporedjivanje sumi...
[verify-global]: Test je prosao :)
[verify-global]: Kraj!

[verify-global]: Iteracija 3:
[verify-global]: Generisanje C programa...
[csmith-gen]: Seme programa je 8945905696653155268.
[csmith-gen]: Generisan je novi program!
[verify-global]: Transformacija c program...
[verify-global]: Uporedjivanje sumi...
[verify-global]: Test je prosao :)
[verify-global]: Kraj!
```

**Objašnjenje:**
U prvom koraku se prevodi biblioteka za transformacije (cmake build), a tek nakon toga se izvršavaju iteracije
metamorfnog testiranja. Svaka iteracija podrazumeva generisanje C programa preko `CSmith` alata. Alat generise
  nasumični C kod (koji ne mora da se razvršava). Izlaz iz ovog programa je suma koji je izračunata nizom
  nekih nasumičnih operacija. Ukoliko se kod sigurno završava (ovo se proverava prevođenjem i pokretanjem), 
  onda se izvršava niz transformacija nad kodom. Nakon toga se transformisani C kod prevodi i pokreće opet.
  Ukoliko se kod uspešno prevodi i daje identičan izlat, onda se smatra da je test prošao. U suprotnom je 
  test pao i čuva se u `storage` direktorijumu.

**Opcije**:
- `verbosity`: Ispisuju se dodatne informacije pri izvršavanju skripti što je korisno pri debagovanju. U ovom slučaju
se ispisuju informacije o izabranim transformacijama nad testu. Veći broj odgovara većem broju detalja.
- `compiler`: Da li se koristi `gcc` ili `clang`.
- `compiler-options`: Opcije kompilatora. Primer: Nivo optimizacija.
- `trans-seq`: Ako je broj, onda se odnosi na broj transformacija po testu. Ako je lista, onda se odnosi
na eksplicitan niz transformacija koje se izvršavaju nad generisanim C kodovima.
- `tests`: Broj testova (iteracija) koje se izvršavaju.
- `max-duration`: Broj sekundi za čekanje programa za izvršavanje. Smatra se da program ima beskonačnu petlju
ako se ne završava u roku od `max-duration` sekundi. Ovo naravno ima netačno pozitivno klasifikovane programe,
ali nema netačno negativno klasifikovane programe što je od velikog praktičnog značaja.
`parallel_cmake_jobs`: Broj paralelnij zadataka za izgradnju `trans` biblioteke.
  
**Primer:**
```
python3 verify.py --max-duration=10 --tests=20 --trans-seq=1 --verbosity=100
```

## Članovi tima :boy:
* [Momir Adžemović](https://github.com/Robotmurlock), 1005/2020
* [Lazar Vasović](https://github.com/matfija), 1011/2020
* [Marko Spasić](https://github.com/spaske00), 1014/2020


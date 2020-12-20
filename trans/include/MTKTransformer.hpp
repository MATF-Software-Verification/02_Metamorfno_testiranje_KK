#ifndef MTKTRANS_H
#define MTKTRANS_H

#include <string>
#include <unordered_set>

/* Klasa koja predstavlja transformator */
class MTKTransformer {
public:
    /* Postavljanje datoteka */
    MTKTransformer(const std::string &s,
                   const std::string &n);

    /* Prijavljivanje greske u radu */
    [[noreturn]] static void greska(const std::string &poruka);

    /* Enumeracija izmena */
    enum Izmena {
        While2Do,
        Do2For,
        While2For,
        PrepFor,
        For2While,
        For2Do,
        Rek2Iter,
        FinIter,
        Iter2Rek,
        FinRek,
        PrepIf,
        PrepSwitch,
        If2Switch,
        Switch2If
    };

    /* Primena zeljene izmene koda; sustinski je
     * boilerplate (sablonski) kod za rad sa AST */
    void primeni(Izmena izmena);

private:
    /* Datoteke koje se obradjuju */
    std::string stara, nova;

    /* Skup obradjenih funkcija */
    std::unordered_set<std::string> fje;

    /* Provera validnosti ulaza */
    void proveri() const;

    /* Informacije o cestim greskama */
    static constexpr auto istaDatoteka = "Datoteke su istog imena!";
    static constexpr auto postojiDatoteka = "Nova datoteka vec postoji!";
    static constexpr auto nemaDatoteke = "Neuspelo otvaranje datoteke!";
    static constexpr auto losUlaz = "Nepremostivi problem sa ulazom!";
};

#endif

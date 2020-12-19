#ifndef MTKTRANS_H
#define MTKTRANS_H

#include <string>

/* Klasa koja predstavlja transformator */
class MTKTransformer {
public:
    /* Postavljanje datoteka */
    MTKTransformer(std::string &s, std::string &n);

    /* Staticko oslobadjanje memorije */
    static void oslobodi();

    /* Prijavljivanje greske u radu */
    [[noreturn]] static void greska(const std::string &poruka);

    /* Enumeracija akcija */
    enum Akcija {
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

    /* Obrada prema zeljenoj akciji; sustinski
     * boilerplate (sablonski) kod za rad sa AST */
    void obradi(Akcija akcija);

private:
    /* Datoteke koje se obradjuju */
    std::string &stara, &nova;

    /* Informacija o cestoj gresci */
    constexpr static auto nemaDatoteke = "Neuspelo otvaranje datoteke!";
};

#endif

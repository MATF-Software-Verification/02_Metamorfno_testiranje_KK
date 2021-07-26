#ifndef MTKTRANS_H
#define MTKTRANS_H

#include "clang/Frontend/CompilerInstance.h"

#include <string>
#include <unordered_set>

using namespace clang;
namespace clang {
class Rewriter;
}

/* Klasa koja predstavlja transformator */
class MTKTransformer {
public:
    /* Postavljanje datoteka */
    MTKTransformer(const std::string &s,
                   const std::string &n);

    /* Enumeracija izmena */
    enum Izmena {
        While2Do,
        Do2For,
        While2For,
        PrepFor,
        For2While,
        For2Do,
        LoopUnroll,
        Rek2Iter,
        FinIter,
        Iter2Rek,
        FinRek,
        PrepIf,
        PrepIff,
        PrepSwitch,
        If2Switch,
        Switch2If,
        CodeImput,
        While2Goto,
        For2Goto,
        Do2Goto,
        PrepWhile2Goto,
        PrepFor2Goto,
        PrepDo2Goto,
        Identitet
    };

    /* Prijavljivanje greske u radu */
    static int greska(const std::string &poruka, bool fatal = false);

    /* Resetovanje statickih parametara */
    static void resetuj();

    /* Obrada zeljene transformacije */
    static int obradi(int argc, const char *argv[]);

private:
    /* Datoteke koje se obradjuju */
    std::string stara, nova;

    /* Skup obradjenih funkcija */
    std::unordered_set<std::string> fje;

    /* Instanca prevodioca */
    CompilerInstance TheCompInst;
    Rewriter *TheRewriter;
    ASTContext *TheASTContext;

    /* Odabir odgovarajuceg transformatora */
    ASTConsumer *odaberiTransformator(Izmena izmena);

    /* Potencijalne izmene koda */
    bool izmeniKod(Izmena izmena);

    /* Provera validnosti ulaza */
    void proveri() const;

    /* Primena zeljene izmene koda; sustinski je
     * boilerplate (sablonski) kod za rad sa AST */
    void primeni(Izmena izmena);

    /* Registrovanje zeljenog broja odmotavanja */
    static void postaviOdmotavanje(unsigned long long n);

    /* Registrovanje zeljenog broja umetanja */
    static void postaviVerovatnocu(unsigned long long n);

    /* Podatak o uocenoj gresci */
    static bool error;

    /* Informacije o cestim greskama */
    static constexpr auto istaDatoteka = "Datoteke su istog imena!";
    static constexpr auto postojiDatoteka = "Nova datoteka vec postoji!";
    static constexpr auto nemaDatoteke = "Neuspelo otvaranje datoteke!";
    static constexpr auto losUlaz = "Nepremostivi problem sa ulazom!";

    /* Nacin upotrebe programa */
    static constexpr auto upotreba = "Upotreba 1: ./trans <ulaz> <izlaz> <do|while|for>\n"
                                     "Upotreba 2: ./trans <ulaz> <izlaz> o<n: uint>\n"
                                     "Upotreba 3: ./trans <ulaz> <izlaz> <if|switch>\n"
                                     "Upotreba 4: ./trans <ulaz> <izlaz> <iter|rek>\n"
                                     "Upotreba 5: ./trans <ulaz> <izlaz> u<n: uint>";
};

#endif

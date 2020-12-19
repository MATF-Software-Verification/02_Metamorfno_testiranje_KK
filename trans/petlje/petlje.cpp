#include "MTKTransformer.hpp"

#include <sstream>

/* Nacin upotrebe programa */
static const auto upotreba = "Upotreba: ./petlje <ulaz> <izlaz> <do|while|for>";

/* Glavna funkcija aplikacije */
int main(int argc, char *argv[]) {
    /* Prekid pogresno pokrenutog programa */
    if (argc != 4) MTKTransformer::greska(upotreba);

    /* Citanje argumenata */
    std::string stara(argv[1]);
    std::string nova(argv[2]);
    std::string petlja(argv[3]);

    /* Postavljanje datoteka */
    MTKTransformer trans(stara, nova);

    /* Prekid pogresno pokrenutog programa */
    if (petlja != "do" &&
        petlja != "while" &&
        petlja != "for")
        MTKContext::greska(upotreba);

    /* Prvi deo algoritma */
    if (petlja == "do")
        trans.obradi(MTKTransformer::While2Do);
    else
        trans.obradi(MTKTransformer::Do2For);

    /* Drugi deo algoritma */
    if (petlja == "for")
        trans.obradi(MTKTransformer::While2For);
    else
        trans.obradi(MTKTransformer::PrepFor);

    /* Treci deo algoritma */
    if (petlja == "while")
        trans.obradi(MTKTransformer::For2While);
    else if (petlja == "do")
        trans.obradi(MTKTransformer::For2Do);

    /* Lepo formatiranje novog koda */
    std::ostringstream buffer;
    buffer << "clang-format -i " << nova;
    std::system(buffer.str().c_str());

    /* Oslobadjanje memorije */
    MTKTransformer::oslobodi();

    /* Normalno zavrsavanje programa */
    exit(EXIT_SUCCESS);
}

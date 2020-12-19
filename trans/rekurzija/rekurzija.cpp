#include "MTKTransformer.hpp"

#include <sstream>

/* Nacin upotrebe programa */
static const auto upotreba = "Upotreba: ./rekurzija <ulaz> <izlaz> <iter|rek>";

/* Glavna funkcija aplikacije */
int main(int argc, char *argv[]) {
    /* Prekid pogresno pokrenutog programa */
    if (argc != 4) MTKTransformer::greska(upotreba);

    /* Citanje argumenata */
    std::string stara(argv[1]);
    std::string nova(argv[2]);
    std::string rekurzija(argv[3]);

    /* Postavljanje datoteka */
    MTKTransformer trans(stara, nova);

    /* Obrada zeljene transformacije */
    if (rekurzija == "iter") {
        trans.obradi(MTKTransformer::Rek2Iter);
        trans.obradi(MTKTransformer::FinIter);
    } else if (rekurzija == "rek") {
        trans.obradi(MTKTransformer::Do2For);
        trans.obradi(MTKTransformer::PrepFor);
        trans.obradi(MTKTransformer::For2While);
        trans.obradi(MTKTransformer::Iter2Rek);
        trans.obradi(MTKTransformer::FinRek);
    /* Prekid pogresno pokrenutog programa */
    } else MTKTransformer::greska(upotreba);

    /* Lepo formatiranje novog koda */
    std::ostringstream buffer;
    buffer << "clang-format -i " << nova;
    std::system(buffer.str().c_str());

    /* Oslobadjanje memorije */
    MTKTransformer::oslobodi();

    /* Normalno zavrsavanje programa */
    exit(EXIT_SUCCESS);
}

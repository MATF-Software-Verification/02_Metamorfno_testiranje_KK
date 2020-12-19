#include "MTKTransformer.hpp"

#include <sstream>

/* Nacin upotrebe programa */
static const auto upotreba = "Upotreba 1: ./trans <ulaz> <izlaz> <do|while|for>\n"
                             "Upotreba 2: ./trans <ulaz> <izlaz> <if|switch>\n"
                             "Upotreba 3: ./trans <ulaz> <izlaz> <iter|rek>";

/* Glavna funkcija aplikacije */
int main(int argc, char *argv[]) {
    /* Prekid pogresno pokrenutog programa */
    if (argc != 4) MTKTransformer::greska(upotreba);

    /* Citanje argumenata */
    std::string stara(argv[1]);
    std::string nova(argv[2]);
    std::string radnja(argv[3]);

    /* Instanciranje transformatora */
    MTKTransformer trans(stara, nova);

    /* Prvi deo algoritma */
    if (radnja == "do") {
        trans.obradi(MTKTransformer::While2Do);
        trans.obradi(MTKTransformer::PrepFor);
        trans.obradi(MTKTransformer::For2Do);
    } else if (radnja == "for") {
        trans.obradi(MTKTransformer::Do2For);
        trans.obradi(MTKTransformer::While2For);
    } else if (radnja == "while") {
        trans.obradi(MTKTransformer::Do2For);
        trans.obradi(MTKTransformer::PrepFor);
        trans.obradi(MTKTransformer::For2While);
    } else if (radnja == "if") {
        trans.obradi(MTKTransformer::PrepSwitch);
        trans.obradi(MTKTransformer::Switch2If);
    } else if (radnja == "switch") {
        trans.obradi(MTKTransformer::PrepIf);
        trans.obradi(MTKTransformer::If2Switch);
    } else if (radnja == "iter") {
        trans.obradi(MTKTransformer::Rek2Iter);
        trans.obradi(MTKTransformer::FinIter);
    } else if (radnja == "rek") {
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

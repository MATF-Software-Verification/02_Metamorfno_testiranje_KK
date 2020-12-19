#include "MTKTransformer.hpp"

#include <sstream>

/* Nacin upotrebe programa */
static constexpr auto upotreba = "Upotreba 1: ./trans <ulaz> <izlaz> <do|while|for>\n"
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
        trans.primeni(MTKTransformer::While2Do);
        trans.primeni(MTKTransformer::PrepFor);
        trans.primeni(MTKTransformer::For2Do);
    } else if (radnja == "for") {
        trans.primeni(MTKTransformer::Do2For);
        trans.primeni(MTKTransformer::While2For);
    } else if (radnja == "while") {
        trans.primeni(MTKTransformer::Do2For);
        trans.primeni(MTKTransformer::PrepFor);
        trans.primeni(MTKTransformer::For2While);
    } else if (radnja == "if") {
        trans.primeni(MTKTransformer::PrepSwitch);
        trans.primeni(MTKTransformer::Switch2If);
    } else if (radnja == "switch") {
        trans.primeni(MTKTransformer::PrepIf);
        trans.primeni(MTKTransformer::If2Switch);
    } else if (radnja == "iter") {
        trans.primeni(MTKTransformer::Rek2Iter);
        trans.primeni(MTKTransformer::FinIter);
    } else if (radnja == "rek") {
        trans.primeni(MTKTransformer::Do2For);
        trans.primeni(MTKTransformer::PrepFor);
        trans.primeni(MTKTransformer::For2While);
        trans.primeni(MTKTransformer::Iter2Rek);
        trans.primeni(MTKTransformer::FinRek);
    /* Prekid pogresno pokrenutog programa */
    } else MTKTransformer::greska(upotreba);

    /* Lepo formatiranje novog koda */
    std::ostringstream buffer;
    buffer << "clang-format -i " << nova;
    std::system(buffer.str().c_str());

    /* Normalno zavrsavanje programa */
    exit(EXIT_SUCCESS);
}

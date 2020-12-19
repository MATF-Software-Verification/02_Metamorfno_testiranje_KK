#include "MTKTransformer.hpp"

#include <sstream>

/* Nacin upotrebe programa */
static const auto upotreba = "Upotreba: ./uslovi <ulaz> <izlaz> <if|switch>";

/* Glavna funkcija aplikacije */
int main(int argc, char *argv[]) {
    /* Prekid pogresno pokrenutog programa */
    if (argc != 4) MTKTransformer::greska(upotreba);

    /* Citanje argumenata */
    std::string stara(argv[1]);
    std::string nova(argv[2]);
    std::string uslov(argv[3]);

    /* Postavljanje datoteka */
    MTKTransformer trans(stara, nova);

    /* Obrada zeljene transformacije */
    if (uslov == "if") {
        trans.obradi(MTKTransformer::PrepSwitch);
        trans.obradi(MTKTransformer::Switch2If);
    } else if (uslov == "switch") {
        trans.obradi(MTKTransformer::PrepIf);
        trans.obradi(MTKTransformer::If2Switch);
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

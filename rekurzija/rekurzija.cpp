/* Ukljucivanje implementiranih klasa */
#include "MTKConsumer.hpp"

/* Enumeracija akcija */
enum class Akcija {
    Iter2Rek,
    Rek2Iter
};

/* Nacin upotrebe programa */
static std::string *stari, *novi, *rekurzija;
const auto upotreba = "Upotreba: ./rekurzija <ulaz> <izlaz> <iter|rek>\n";

/* Obrada prema zeljenoj akciji; sustinski
 * boilerplate (sablonski) kod za rad sa AST */
static void obradi(const Akcija &) {}

/* Glavna funkcija aplikacije */
int main(int argc, char *argv[]) {
    /* Prekid pogresno pokrenutog programa */
    if (argc != 4) {
        llvm::errs() << upotreba;
        exit(EXIT_FAILURE);
    }

    /* Citanje argumenata */
    stari = new std::string(argv[1]);
    novi = new std::string(argv[2]);
    rekurzija = new std::string(argv[3]);

    /* Obrada zeljene transformacije */
    if (*rekurzija == "iter") {
        obradi(Akcija::Rek2Iter);
    } else if (*rekurzija == "rek") {
        obradi(Akcija::Iter2Rek);
    /* Prekid pogresno pokrenutog programa */
    } else {
        llvm::errs() << upotreba;
        exit(EXIT_FAILURE);
    }

    /* Lepo formatiranje novog koda */
    std::ostringstream buffer;
    buffer << "clang-format -i " << *novi;
    std::system(buffer.str().c_str());

    /* Oslobadjanje memorije */
    delete stari;
    delete novi;
    delete rekurzija;

    /* Normalno zavrsavanje programa */
    exit(EXIT_SUCCESS);
}

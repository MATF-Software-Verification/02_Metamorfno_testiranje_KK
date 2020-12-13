/* Ukljucivanje implementiranih klasa */
#include "MTKConsumer.hpp"

/* Enumeracija akcija */
enum class Akcija {
    PrepIf,
    PrepSwitch,
    If2Switch,
    Switch2If
};

/* Nacin upotrebe programa */
static std::string *stari, *novi, *uslov;
const auto upotreba = "Upotreba: ./uslovi <ulaz> <izlaz> <if|switch>\n";

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
    uslov = new std::string(argv[3]);

    /* Obrada zeljene transformacije */
    if (*uslov == "if") {
        obradi(Akcija::PrepSwitch);
        obradi(Akcija::Switch2If);
    } else if (*uslov == "switch") {
        obradi(Akcija::PrepIf);
        obradi(Akcija::If2Switch);
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
    delete uslov;

    /* Normalno zavrsavanje programa */
    exit(EXIT_SUCCESS);
}

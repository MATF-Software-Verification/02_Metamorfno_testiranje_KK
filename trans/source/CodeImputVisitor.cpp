#include "CodeImputVisitor.hpp"

#include <fstream>
#include <sstream>

/***********************
 * Shema transformacije
 * ---------------------
 * if (uslov)
 *   telo;
 * ---------------------
 * noviKod1;
 * if (uslov) {
 *   noviKod2;
 *   telo;
 *   noviKod3;
 * }
 * noviKod4;
 ***********************/

/* Staticki podatak o verovatnoci umetanja */
unsigned long long CodeImputVisitor::n = 1;

/* Staticki spisak datoteka za dodavanje */
const char *CodeImputVisitor::dats[] = {
    "../trans/umetanje/bezopt.txt",
    "../trans/umetanje/undefopt.txt"
};

/* Postavljanje broja umetanja */
void CodeImputVisitor::postaviBroj(unsigned long long i) {
    if (i > 0) n = i;
}

/* Imputacija oko naredbe ako treba */
bool CodeImputVisitor::VisitStmt(Stmt *s) {
    /* Odustajanje ako je u tabu skupu */
    if (tabu.count(s)) return true;

    /* Odabir datoteke sa tekstom */
    const auto i = static_cast<size_t>(rand()) %
                  (sizeof dats / sizeof(char *));
    std::string dat = dats[i];

    /* Pokusaj otvaranja datoteke */
    std::ifstream ulaz(dat);
    if (!ulaz) {
        dat = dat.substr(dat.find_last_of('/')+1);
        ulaz.open(dat);
    }
    if (!ulaz) greska(nemaDatoteke);

    /* Izvlacenje teksta iz datoteke */
    std::ostringstream buffer;
    buffer << ulaz.rdbuf();
    const auto tekst = buffer.str();

    /* Tekstualna zamena koda */
    dodajIspred(s, tekst); dodajIza(s);

    /* Nastavljanje dalje */
    return true;
}

/* Obilazak svih naredbi */
bool CodeImputVisitor::TraverseStmt(Stmt *s) {
    /* Verovatnoca izmene je 1/n */
    const auto izmena = s && !isa<ValueStmt>(s) && !isa<DeclStmt>(s) &&
                        static_cast<unsigned long long>(rand()) % n == 0;

    /* Nastavljanje dalje ili odustajanje */
    if (izmena) return WalkUpFromStmt(s);
    else {
        tabu.insert(s);
        return RecursiveASTVisitor<CodeImputVisitor>::TraverseStmt(s);
    }
}

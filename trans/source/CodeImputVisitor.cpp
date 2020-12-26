#include "CodeImputVisitor.hpp"

#include "clang/AST/ParentMapContext.h"

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
    "../trans/umetanje/lagrneopt.txt",
    "../trans/umetanje/undefined.txt",
    "../trans/umetanje/lagrundef.txt"
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

/* Provera da li je moguca izmena */
bool CodeImputVisitor::nemogucaIzmena(Stmt *s) const {
    /* U redu je ako nema vrednost */
    const auto val = dyn_cast<ValueStmt>(s);
    if (!val) return false;

    /* Nije u redu ako nema roditelja */
    const auto rod = TheASTContext.getParents(*s);
    if (rod.empty()) return true;

    /* U redu je ako je roditelj slozen */
    return !rod.begin()->get<CompoundStmt>();
}

/* Obilazak svih naredbi */
bool CodeImputVisitor::TraverseStmt(Stmt *s) {
    /* Odustajanje od deklaracija ili izraza koji
     * nisu u slozenoj naredbi, kao i nultih naredbi */
    if (!s || isa<DeclStmt>(s) || nemogucaIzmena(s)) {
        tabu.insert(s); return WalkUpFromStmt(s);
    }

    /* Verovatnoca izmene je 1/n, pri cemu se mogu menjati samo one
     * naredbe koje nisu izrazi, sa izuzetkom poziva i deklaracija */
    const auto izmena = static_cast<unsigned long long>(rand()) % n == 0;

    /* Nastavljanje dalje i oznacavanje ukoliko nema izmena */
    if (!izmena) tabu.insert(s);
    return RecursiveASTVisitor<CodeImputVisitor>::TraverseStmt(s);
}

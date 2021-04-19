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
    "../test/umetanje/lagrneopt.txt",
    "../test/umetanje/undefined.txt",
    "../test/umetanje/lagrundef.txt"
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
        dat = dat.substr(dat.find('/')+1);
        ulaz.open(dat);
    }
    if (!ulaz) {
        dat = dat.substr(dat.rfind('/')+1);
        ulaz.open(dat);
    }
    if (!ulaz) greska(nemaDatoteke, true);

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
    if (!isa<ValueStmt>(s)) return false;

    /* Nije u redu ako nema roditelja */
    const auto r = rods.at(s);
    if (!r) return true;

    /* U redu je ako je roditelj slozen */
    return !isa<CompoundStmt>(r);
}

/* Obilazak svih naredbi */
bool CodeImputVisitor::TraverseStmt(Stmt *s) {
    /* Odustajanje od deklaracija ili izraza koji
     * nisu u slozenoj naredbi, kao i nultih naredbi */
    if (!s || isa<DeclStmt>(s) || nemogucaIzmena(s)) {
        tabu.insert(s); return WalkUpFromStmt(s);
    }

    /* Verovatnoca izmene je 1/n */
    const auto neizm = static_cast<unsigned long long>(rand()) % n;

    /* Nastavljanje dalje i oznacavanje ukoliko nema izmena */
    if (neizm) tabu.insert(s);
    return MTKVisitor::TraverseStmt(s);
}

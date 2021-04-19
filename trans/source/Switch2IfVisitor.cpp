#include "Switch2IfVisitor.hpp"

/*******************************
 * Shema transformacije
 * -----------------------------
 * switch (uslov) {
 *   case a:
 *     teloa;
 *
 *   case b:
 *     telob;
 *
 *   default:
 *     telodef;
 *
 *   case c:
 *     teloc;
 *
 *   case d:
 *     telod;
 * }
 * -----------------------------
 * int cond = uslov;
 * do {
 *   if (cond == a)
 *     teloa;
 *
 *   if (cond == a || cond == b)
 *     telob;
 *
 *   if (cond != c && cond != d)
 *     telodef;
 *
 *   if (cond != d)
 *     teloc;
 *
 *   telod;
 * } while (0);
 *******************************/

/* Izracunavanje uslova za default */
Expr *Switch2IfVisitor::defUslov(StmtIterator dete,
                                 StmtIterator kraj,
                                 DeclRefExpr *uslov,
                                 std::deque<Expr *> &u) const {
    /* Inicijalizacija uslova */
    Expr *cond = nullptr;

    /* Popunjavanje svim nejednakostima */
    for (; dete != kraj; dete++) {
        const auto cas = dyn_cast<CaseStmt>(*dete);
        if (!cas) continue; /* mora biti case */

        /* Izracunavanje tekuce nejednakosti */
        u.push_back(napraviNejednakost(uslov, cas->getLHS()));

        /* Konjukcija nejednakosti ako ih ima vise */
        cond = cond ? napraviKonjunkciju(cond, u.back()) : u.back();
    }

    /* Vracanje rezultata */
    return cond;
}

/* Provera da li je neprazan case */
bool Switch2IfVisitor::neprazanSwitchCase(SwitchCase *s) const {
    return s && !isa<BreakStmt>(s->getSubStmt());
}

/* Provera ima li dubokih oznaka */
bool Switch2IfVisitor::dubokeOznake(SwitchStmt *s) const {
    /* Prolazak kroz svaki switch case */
    for (auto swc = s->getSwitchCaseList();
         swc; swc = swc->getNextSwitchCase()) {
        /* Dohvatanje prvog roditelja */
        const auto telo = dyn_cast<CompoundStmt>(rods.at(swc));
        if (!telo) return true;

        /* Dohvatanje drugog roditelja */
        const auto swch = dyn_cast<SwitchStmt>(rods.at(telo));
        if (!swch) return true;
    }

    /* Sve je u redu u ovom trenutku */
    return false;
}

/* Pretvaranje switch naredbe u if */
bool Switch2IfVisitor::VisitSwitchStmt(SwitchStmt *s) const {
    /* Odustajanje ako postoje duboke oznake */
    if (dubokeOznake(s)) return true;

    /* Uslovna promenljiva switcha */
    const auto dekl = napraviUslovnu(tekdek, "cond", false);

    /* Deklaracija sa prepisanim uslovom */
    cast<VarDecl>(dekl->getSingleDecl())->setInit(s->getCond());

    /* Uslovna promenljiva kao izraz */
    const auto uslov = napraviDeclExpr(dekl);

    /* Nizovi uslova i if naredbi */
    std::vector<Expr *> disj;
    std::deque<Expr *> konj;
    std::vector<Stmt *> telo;

    /* Telo switcha kroz koje se iterira */
    const auto start = std::cbegin(s->getBody()->children());
    const auto kraj = std::cend(s->getBody()->children());

    /* Prolazak kroz telo switcha */
    auto dete = start;
    for (; dete != kraj; dete++) {
        /* Case naredba: dodavanje uslova jednakosti */
        if (const auto cas = dyn_cast<CaseStmt>(*dete)) {
            disj.push_back(napraviJednakost(uslov, cas->getLHS()));

            /* Disjunkcija svih dosadasnjih uslova */
            auto cond = disj.front();
            for (auto i = 1ul; i < disj.size(); i++) {
                cond = napraviDisjunkciju(cond, disj[i]);
            }

            /* Pravljenje odgovarajuce if naredbe */
            telo.push_back(napraviIf(cond, cas->getSubStmt()));
        /* Default naredba: posebno sracunat uslov */
        } else if (const auto def = dyn_cast<DefaultStmt>(*dete)) {
            const auto defUsl = defUslov(dete, kraj, uslov, konj);

            /* Ima uslova -> dodavanje if naredbe */
            if (defUsl) telo.push_back(napraviIf(defUsl, def->getSubStmt()));
            /* Nema uslova -> dodavanje ako nije break */
            else if (neprazanSwitchCase(def))
                telo.push_back(def->getSubStmt());

            /* Iskakanje iz petlje nakon default */
            break;
        /* Ostale naredbe: dodavanje na kraj prethodnog ifa */
        } else {
            const auto iff = cast<IfStmt>(telo.back());
            iff->setThen(napraviSlozenu({iff->getThen(), *dete}));
        }
    }

    /* Pomeranje iteratora ako moze */
    if (dete != kraj) dete++;

    /* Prolazak kroz ostatak tela */
    for (; dete != kraj; dete++) {
        /* Case naredba: uklanjanje uslova nejednakosti */
        if (const auto cas = dyn_cast<CaseStmt>(*dete)) {
            konj.pop_front();

            /* Samo se telo dodaje ako nema dalje
             * i ako nije prazno (npr. samo break) */
            if (konj.empty()) {
                if (neprazanSwitchCase(cas))
                    telo.push_back(cas->getSubStmt());
                continue;
            }

            /* Konjunkcija svih preostalih uslova */
            auto cond = konj.front();
            for (auto i = 1ul; i < konj.size(); i++) {
                cond = napraviKonjunkciju(cond, konj[i]);
            }

            /* Pravljenje odgovarajuce if naredbe */
            telo.push_back(napraviIf(cond, cas->getSubStmt()));
        /* Ostale naredbe: dodavanje na kraj prethodnog ifa */
        } else if (const auto iff = dyn_cast<IfStmt>(telo.back())) {
            iff->setThen(napraviSlozenu({iff->getThen(), *dete}));
        /* Ili prosto dodavanje na kraj ako nema ifa */
        } else telo.push_back(*dete);
    }

    /* Transformisano telo */
    const auto ttelo = napraviSlozenu(telo);

    /* Do petlja kao omotac svega */
    const auto petlja = napraviDo(ttelo, napraviFalse());

    /* Slozena naredba za zamenu */
    const auto zamena = napraviSlozenu({dekl, petlja});

    /* Tekstualna zamena koda */
    zameni(s, zamena);

    /* Nastavljanje dalje */
    return true;
}

/* Prekid obilaska kod switch naredbe */
bool Switch2IfVisitor::TraverseSwitchStmt(SwitchStmt *s) {
    return !dubokeOznake(s) ? WalkUpFromSwitchStmt(s) :
           MTKVisitor::TraverseSwitchStmt(s);
}

/* Nacin obrade deklaracije */
bool Switch2IfVisitor::TraverseDecl(Decl *d) {
    /* Privatno cuvanje tekuce deklaracije */
    tekdek = d;

    /* Nastavljanje dalje */
    return MTKVisitor::TraverseDecl(d);
}

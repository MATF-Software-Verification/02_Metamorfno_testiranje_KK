#include "Switch2IfVisitor.hpp"

/* Shema transformacije
 * --------------------------------------------------------
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
 * --------------------------------------------------------
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
 *   if (cond == a || cond == b || cond == c)
 *     teloc;
 *
 *   if (cond == a || cond == b || cond == c || cond == d)
 *     telod;
 * } while (0);
 * */

/* Izracunavanje uslova za default */
Expr *Switch2IfVisitor::defUslov(StmtIterator dete,
                                 StmtIterator kraj,
                                 DeclRefExpr *uslov) const {
    /* Inicijalizacija uslova */
    Expr *cond = nullptr;

    /* Popunjavanje svim nejednakostima */
    for (; dete != kraj; dete++) {
        const auto cas = dyn_cast<CaseStmt>(*dete);

        /* Konjukcija nejednakosti ako ih ima vise */
        if (cas) cond = !cond ? napraviNejednakost(uslov, cas->getLHS()) :
        napraviKonjunkciju(cond, napraviNejednakost(uslov, cas->getLHS()));
    }

    /* Vracanje rezultata */
    return cond;
}

/* Provera da li je prazan default */
bool Switch2IfVisitor::prazanDefault(DefaultStmt *s) const {
    return !s || isa<BreakStmt>(s->getSubStmt());
}

/* Pretvaranje switch naredbe u if */
bool Switch2IfVisitor::VisitSwitchStmt(SwitchStmt *s) const {
    /* Uslovna promenljiva switcha */
    const auto dekl = napraviUslovnu(tekdek, "cond", false);

    /* Deklaracija sa prepisanim uslovom */
    cast<VarDecl>(dekl->getSingleDecl())->setInit(s->getCond());

    /* Uslovna promenljiva kao izraz */
    const auto uslov = napraviDeclExpr(dekl);

    /* Nizovi uslova i if naredbi */
    std::vector<Expr *> conds;
    std::vector<Stmt *> ifovi;

    /* Telo switcha kroz koje se iterira */
    const auto start = std::cbegin(s->getBody()->children());
    const auto kraj = std::cend(s->getBody()->children());

    /* Iteracija kroz telo switcha */
    for (auto dete = start; dete != kraj; dete++) {
        /* Case naredba: dodavanje uslova jednakosti */
        if (auto cas = dyn_cast<CaseStmt>(*dete)) {
            conds.push_back(napraviJednakost(uslov, cas->getLHS()));

            /* Disjunkcija svih dosadasnjih uslova */
            Expr *cond = conds[0];
            for (auto i = 1ul; i < conds.size(); i++) {
                cond = napraviDisjunkciju(cond, conds[i]);
            }

            /* Pravljenje odgovarajuce if naredbe */
            ifovi.push_back(napraviIf(cond, cas->getSubStmt()));
        /* Default naredba: posebno sracunat uslov */
        } else if (auto def = dyn_cast<DefaultStmt>(*dete)) {
            const auto defUsl = defUslov(dete, kraj, uslov);

            /* Ima uslova -> dodavanje if naredbe */
            if (defUsl)
                ifovi.push_back(napraviIf(defUsl, def->getSubStmt()));
            /* Nema uslova -> dodavanje ako nije break */
            else if (!prazanDefault(def))
                ifovi.push_back(def->getSubStmt());
        /* Ostale naredbe: dodavanje na kraj prethodnog ifa */
        } else {
            const auto iff = cast<IfStmt>(ifovi.back());
            iff->setThen(napraviSlozenu({iff->getThen(), *dete}));
        }
    }

    /* Transformisano telo */
    const auto telo = napraviSlozenu(ifovi);

    /* Do petlja kao omotac svega */
    const auto petlja = napraviDo(telo, napraviFalse());

    /* Slozena naredba za zamenu */
    const auto zamena = napraviSlozenu({dekl, petlja});

    /* Tekstualna zamena koda */
    zameni(s, zamena);

    /* Nastavljanje dalje */
    return true;
}

/* Prekid obilaska kod switch naredbe */
bool Switch2IfVisitor::TraverseSwitchStmt(SwitchStmt *s) {
    return WalkUpFromSwitchStmt(s);
}

/* Nacin obrade deklaracije */
bool Switch2IfVisitor::TraverseDecl(Decl *d) {
    /* Privatno cuvanje tekuce deklaracije */
    tekdek = d;

    /* Nastavljanje dalje */
    return RecursiveASTVisitor<Switch2IfVisitor>::TraverseDecl(d);
}

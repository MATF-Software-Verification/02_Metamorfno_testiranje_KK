#include "Switch2IfVisitor.hpp"

/* Izracunavanje uslova za default */
Expr *Switch2IfVisitor::defUslov(StmtIterator d, SwitchStmt *s, DeclRefExpr *u) {
    Expr *cond = nullptr;
    for (; d != s->getBody()->children().end(); d++) {
        const auto cas = dyn_cast<CaseStmt>(*d);
        if (cas) {
            if (cond)
                cond = napraviKonjunkciju(cond, napraviNejednakost(u, cas->getLHS()));
            else
                cond = napraviNejednakost(u, cas->getLHS());
        }
    }
    return cond;
}

/* Pretvaranje switch naredbe u if */
bool Switch2IfVisitor::VisitSwitchStmt(SwitchStmt *s) {
    /* Uslovna promenljiva switcha */
    const auto dekl = napraviUslovnu(tekdek, "cond", false);

    /* Deklaracija sa duplom nagacijom */
    cast<VarDecl>(dekl->getSingleDecl())
        ->setInit(napraviNegaciju(napraviNegaciju(s->getCond())));

    /* Uslovna promenljiva kao izraz */
    const auto uslov = napraviDeclExpr(dekl);

    /* Sakupljanje podataka o switchu */
    std::vector<Expr *> conds;

    std::vector<Stmt *> ifovi;
    for (auto dete = s->getBody()->children().begin(); dete != s->getBody()->children().end(); dete++) {
        if (auto cas = dyn_cast<CaseStmt>(*dete)) {
            conds.push_back(napraviJednakost(uslov, cas->getLHS()));

            Expr *cond = conds[0];
            for (auto i = 1ul; i < conds.size(); i++) {
                cond = napraviDisjunkciju(cond, conds[i]);
            }

            ifovi.push_back(napraviIf(cond, cas->getSubStmt()));
        } else if (auto def = dyn_cast<DefaultStmt>(*dete)) {
            const auto defUsl = defUslov(dete, s, uslov);
            if (defUsl)
                ifovi.push_back(napraviIf(defUsl, def->getSubStmt()));
            else if (!isa<BreakStmt>(def->getSubStmt()))
                ifovi.push_back(def->getSubStmt());
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

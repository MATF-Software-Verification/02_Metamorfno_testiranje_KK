#include "MTKContext.hpp"

#include "clang/Lex/Preprocessor.h"

#include <climits>

/* Tekstualna reprezentacija naredbe */
std::string MTKContext::stampaj(const Stmt *const s) const {
    /* Inicijalizacija izlaznog toka */
    std::string stmt;
    llvm::raw_string_ostream stream(stmt);

    /* Stampanje naredbe u tok */
    s->printPretty(stream, nullptr, PrintingPolicy(LangOptions()));
    stream.flush();

    /* Vracanje rezultata */
    return stmt;
}

/* Odredjivanje mesta naredbe u kodu */
SourceRange MTKContext::odrediMesto(const Stmt *const s, bool dir) const {
    /* Granicne oznake u kodu */
    const auto start = s->getSourceRange().getBegin();
    const auto end = s->getSourceRange().getEnd();

    /* Dohvatanje poslednjeg tokena */
    Token tok;
    Lexer::getRawToken(end, tok, TheRewriter.getSourceMgr(),
                       TheRewriter.getLangOpts());
    const std::string ime = tok.getName();

    /* Racunanje offseta osim ukoliko je kraj slozene naredbe; tada
     * je tacka-zapeta ili zatvorena velika zagrada suvisni token */
    const auto offset = Lexer::MeasureTokenLength(end,
                                                  TheRewriter.getSourceMgr(),
                                                  TheRewriter.getLangOpts())
                        + (!dir && ime != "r_brace" && ime != "semi");

    /* Vracanje tacno izracunatog mesta */
    return SourceRange(start, end.getLocWithOffset(static_cast<int>(offset)));
}

/* Tekstualna zamena koda */
void MTKContext::zameni(const Stmt *const stari,
                        const Stmt *const novi,
                        bool dir) const {
    /* Odredjivanje mesta naredbe u kodu */
    const auto mesto = odrediMesto(stari, dir);

    /* Tekstualna reprezentacija novog iskaza */
    const auto stmt = stampaj(novi);

    /* Promena teksta na izracunatom mestu */
    TheRewriter.ReplaceText(mesto, stmt);
}

/* Prednja tekstualna dopuna koda */
void MTKContext::dodajIspred(const Stmt *const stari,
                             const Stmt *const novi) const {
    /* Odredjivanje mesta naredbe u kodu */
    const auto mesto = odrediMesto(stari);

    /* Tekstualna reprezentacija novog iskaza */
    const auto stmt = "{" + stampaj(novi);

    /* Dodavanje teksta na izracunatom mestu */
    TheRewriter.InsertTextBefore(mesto.getBegin(), stmt);
}

/* Zadnja tekstualna dopuna koda */
void MTKContext::dodajIza(const Stmt *const stari,
                          const Stmt *const novi) const {
    /* Odredjivanje mesta naredbe u kodu */
    const auto mesto = odrediMesto(stari);

    /* Tekstualna reprezentacija novog iskaza */
    const auto stmt = "\n" + stampaj(novi) + "}";

    /* Dodavanje teksta na izracunatom mestu */
    TheRewriter.InsertTextBefore(mesto.getEnd(), stmt);
}

/* Pronalazak prvog slobodnog imena */
std::string MTKContext::nadjiIme(const std::string &pocetno) const {
    /* Pomocne promenljive za pretragu */
    std::string ime(pocetno);
    auto i = 0ull; /* brojac */

    /* Proba svih mogucih kombinacija */
    while (TheASTContext.Idents.find(ime)
           != TheASTContext.Idents.end()) {
        if (i == ULLONG_MAX) exit(EXIT_FAILURE);
        ime = pocetno + std::to_string(i++);
    }

    /* Vracanje pronadjenog imena */
    return ime;
}

/* Pravljenje nove deklaracije */
VarDecl *MTKContext::napraviDecl(DeclContext *kontekst,
                                 const CanQual<Type> &tip,
                                 const std::string &ime) const {
    return VarDecl::Create(TheASTContext, kontekst,
                           SourceLocation(), SourceLocation(),
                           &TheASTContext.Idents.getOwn(ime),
                           tip, nullptr, SC_None);
}

/* Pravljenje izraza deklaracije */
DeclRefExpr *MTKContext::napraviDeclExpr(DeclStmt *deknar) const {
    const auto dekl = cast<VarDecl>(deknar->getSingleDecl());
    return DeclRefExpr::Create(TheASTContext, NestedNameSpecifierLoc(),
                               SourceLocation(), dekl, true,
                               SourceLocation(), dekl->getType(), VK_LValue);
}

/* Pravljenje celobrojne vrednosti */
IntegerLiteral *MTKContext::napraviInt(unsigned long long val) const {
    const auto tip = TheASTContext.IntTy;
    llvm::APInt APValue(static_cast<unsigned>(TheASTContext.getTypeSize(tip)), val);
    return IntegerLiteral::Create(TheASTContext, APValue,
                                  tip, SourceLocation());
}

/* Pravljenje tacne istinitosne vrednosti */
IntegerLiteral *MTKContext::napraviTrue() const {
    return napraviInt(1);
}

/* Pravljenje netacne istinitosne vrednosti */
IntegerLiteral *MTKContext::napraviFalse() const {
    return napraviInt(0);
}

/* Pravljenje deklaracije uslovne promenljive */
DeclStmt *MTKContext::napraviUslovnu(Decl *deklaracija,
                                     const std::string &ime,
                                     const bool pocetna) const {
    /* Pronalazak prvog slobodnog imena */
    const auto slobime = nadjiIme(ime);

    /* Deklaracija uslovne promenljive */
    const auto tip = TheASTContext.IntTy;
    auto dekl = napraviDecl(deklaracija->getDeclContext(), tip, slobime);

    /* Celobrojna vrednost za inicijalizaciju */
    dekl->setInit(napraviInt(pocetna));

    /* Naredba deklaracije uslovne promenljive */
    return naHip(DeclStmt(DeclGroupRef(dekl), SourceLocation(), SourceLocation()));
}

/* Pravljenje izraza u zagradi */
ParenExpr *MTKContext::napraviParen(Expr *izraz) const {
    return naHip(ParenExpr(SourceLocation(), SourceLocation(), izraz));
}

/* Pravljenje unarnog operatora */
UnaryOperator *MTKContext::napraviUnarni(Expr *izraz,
                                         const UnaryOperator::Opcode &op,
                                         const QualType &tip) const {
    /* Stavljanje binarnih operatora u zagradu */
    if (isa<BinaryOperator>(izraz))
        izraz = napraviParen(izraz);

    /* Pravljenje odgovarajuceg unarnog operatora */
    return UnaryOperator::Create(TheASTContext, izraz, op,
                                 tip, VK_RValue, OK_Ordinary,
                                 SourceLocation(), false,
                                 FPOptionsOverride());
}

/* Pravljenje logicke negacije */
UnaryOperator *MTKContext::napraviNegaciju(Expr *izraz) const {
    return napraviUnarni(izraz, UO_LNot, izraz->getType());
}

/* Dohvatanje celobrojne vrednosti */
Expr *MTKContext::dohvatiCelobrojnu(Expr *izraz) const {
    /* Stavljanje binarnih operatora u zagradu */
    if (isa<BinaryOperator>(izraz))
        izraz = napraviParen(izraz);

    /* Izdvajanje podataka o celobrojnom tipu */
    const auto tip = TheASTContext.IntTy;
    const auto tsi = TheASTContext.getTrivialTypeSourceInfo(tip, SourceLocation());

    /* Kastovanje u celobrojni klasicni int */
    return CStyleCastExpr::Create(TheASTContext, tip, VK_RValue,
                                  CK_IntegralCast, izraz, nullptr, tsi,
                                  SourceLocation(), SourceLocation());
}

/* Dohvatanje istinitosne vrednosti */
Expr *MTKContext::dohvatiIstinitost(Expr *izraz) const {
    /* Dohvatanje podataka o tipu izraza */
    const auto integer = izraz->getType()->isIntegerType();
    const auto boolean = izraz->isKnownToHaveBooleanValue(true);

    /* Istinitosni tip se pretvara u ceo broj */
    if (boolean) return dohvatiCelobrojnu(izraz);
    /* Celobrojni neistinitosni tip se ne menja */
    else if (integer) return izraz;
    /* Necelobrojni tip duplo negira i kastuje */
    else return dohvatiCelobrojnu(napraviNegaciju(napraviNegaciju(izraz)));
}

/* Pravljenje binarnog operatora */
BinaryOperator *MTKContext::napraviBinarni(Expr *lhs, Expr *rhs,
                                           const BinaryOperator::Opcode &op,
                                           const QualType &tip) const {
    return BinaryOperator::Create(TheASTContext, lhs, rhs, op,
                                  tip, VK_RValue, OK_Ordinary,
                                  SourceLocation(), FPOptionsOverride());
}

/* Pravljenje izraza dodele */
BinaryOperator *MTKContext::napraviDodelu(Expr *lhs, Expr *rhs) const {
    return napraviBinarni(lhs, rhs, BO_Assign, rhs->getType());
}

/* Pravljenje izraza jednakosti */
BinaryOperator *MTKContext::napraviJednakost(Expr *lhs, Expr *rhs) const {
    return napraviBinarni(lhs, rhs, BO_EQ, rhs->getType());
}

/* Pravljenje izraza nejednakosti */
BinaryOperator *MTKContext::napraviNejednakost(Expr *lhs, Expr *rhs) const {
    return napraviBinarni(lhs, rhs, BO_NE, rhs->getType());
}

/* Pravljenje logicke konjunkcije */
BinaryOperator *MTKContext::napraviKonjunkciju(Expr *lhs, Expr *rhs) const {
    return napraviBinarni(lhs, rhs, BO_LAnd, rhs->getType());
}

/* Pravljenje logicke disjunkcije */
BinaryOperator *MTKContext::napraviDisjunkciju(Expr *lhs, Expr *rhs) const {
    return napraviBinarni(lhs, rhs, BO_LOr, rhs->getType());
}

/* Pravljenje slozene naredbe */
CompoundStmt *MTKContext::napraviSlozenu(const std::vector<Stmt *> &naredbe) const {
    return CompoundStmt::Create(TheASTContext, naredbe,
                                SourceLocation(), SourceLocation());
}

/* Pravljenje uslovne naredbe */
IfStmt *MTKContext::napraviIf(Expr *ako, Stmt *onda, Stmt *inace) const {
    return IfStmt::Create(TheASTContext, SourceLocation(),
                          false, nullptr, nullptr, ako,
                          onda, SourceLocation(), inace);
}

/* Pravljenje do petlje */
DoStmt *MTKContext::napraviDo(Stmt *telo, Expr *uslov) const {
    return naHip(DoStmt(telo, uslov, SourceLocation(),
                        SourceLocation(), SourceLocation()));
}

/* Pravljenje while petlje */
WhileStmt *MTKContext::napraviWhile(Expr *uslov, Stmt *telo) const {
    return WhileStmt::Create(TheASTContext, nullptr, uslov, telo,
                             SourceLocation(), SourceLocation(), SourceLocation());
}

/* Pravljenje for petlje */
ForStmt *MTKContext::napraviFor(Expr *uslov, Expr *korak, Stmt *telo) const {
    return naHip(ForStmt(TheASTContext, nullptr, uslov, nullptr, korak, telo,
                         SourceLocation(), SourceLocation(), SourceLocation()));
}

/* Pravljenje continue naredbe */
ContinueStmt *MTKContext::napraviCont() const {
    return naHip(ContinueStmt(SourceLocation()));
}

/* Pravljenje break naredbe */
BreakStmt *MTKContext::napraviBreak() const {
    return naHip(BreakStmt(SourceLocation()));
}

/* Pravljenje case klauze */
CaseStmt *MTKContext::napraviCase(Expr *izraz, Stmt *naredba) const {
    const auto cas = CaseStmt::Create(TheASTContext, izraz, nullptr,
                         SourceLocation(), SourceLocation(), SourceLocation());
    cas->setSubStmt(naredba); return cas;
}

/* Pravljenje default klauze */
DefaultStmt *MTKContext::napraviDefault(Stmt *naredba) const {
    return naHip(DefaultStmt(SourceLocation(), SourceLocation(), naredba));
}

/* Pravljenje switch naredbe */
SwitchStmt *MTKContext::napraviSwitch(Expr *uslov, Stmt *telo) const {
    const auto sw = SwitchStmt::Create(TheASTContext, nullptr, nullptr, uslov);
    sw->setBody(telo); return sw;
}

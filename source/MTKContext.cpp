#include "MTKContext.hpp"

#include "clang/Lex/Preprocessor.h"

#include <climits>

/* Poruka da nema slobodnog imena */
static const auto nemaImena = "Nije moguce pronaci slobodno ime!";

/* Staticko cuvanje posecenih funkcija */
std::unordered_set<std::string> *MTKContext::fje
    = new std::unordered_set<std::string>;

/* Staticko oslobadjanje memorije */
void MTKContext::oslobodi() {
    delete fje;
}

/* Prijavljivanje greske u radu */
void MTKContext::greska(const std::string &poruka) {
    llvm::errs() << poruka << '\n';
    exit(EXIT_FAILURE);
}

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

    /* Tekstualna reprezentacija nove naredbe */
    const auto stmt = stampaj(novi);

    /* Promena teksta na izracunatom mestu */
    TheRewriter.ReplaceText(mesto, stmt);
}

/* Prednja tekstualna dopuna koda */
void MTKContext::dodajIspred(const Stmt *const stari,
                             const Stmt *const novi) const {
    /* Odredjivanje mesta naredbe u kodu */
    const auto mesto = odrediMesto(stari);

    /* Tekstualna reprezentacija nove naredbe */
    const auto stmt = "{" + stampaj(novi);

    /* Dodavanje teksta na izracunatom mestu */
    TheRewriter.InsertTextBefore(mesto.getBegin(), stmt);
}

/* Zadnja tekstualna dopuna koda */
void MTKContext::dodajIza(const Stmt *const stari,
                          const Stmt *const novi) const {
    /* Odredjivanje mesta naredbe u kodu */
    const auto mesto = odrediMesto(stari);

    /* Tekstualna reprezentacija nove naredbe */
    const auto stmt = "\n" + stampaj(novi) + "}";

    /* Dodavanje teksta na izracunatom mestu */
    TheRewriter.InsertTextBefore(mesto.getEnd(), stmt);
}

/* Tekstualna dopuna koda funkcijom */
void MTKContext::dodajFunkciju(const FunctionDecl *const stara,
                               const FunctionDecl *const nova) const {
    /* Odredjivanje mesta stare funkcije */
    const auto mesto = stara->getSourceRange().getBegin();

    /* Inicijalizacija izlaznog toka */
    std::string fja;
    llvm::raw_string_ostream stream(fja);

    /* Stampanje naredbe u tok */
    nova->print(stream);
    stream.write('\n');
    stream.flush();

    /* Dodavanje teksta na izracunatom mestu */
    TheRewriter.InsertTextBefore(mesto, fja);
}

/* Pronalazak prvog slobodnog imena */
std::string MTKContext::nadjiIme(const std::string &pocetno) const {
    /* Pomocne promenljive za pretragu */
    std::string ime(pocetno);
    auto i = 0ull; /* brojac */

    /* Proba svih mogucih kombinacija */
    while (TheASTContext.Idents.find(ime)
           != TheASTContext.Idents.end()) {
        if (i == ULLONG_MAX) greska(nemaImena);
        ime = pocetno + std::to_string(i++);
    }

    /* Vracanje pronadjenog imena */
    return ime;
}

/* Pravljenje nove promenljive */
VarDecl *MTKContext::napraviVar(DeclContext *kontekst,
                                const QualType &tip,
                                const std::string &ime) const {
    return VarDecl::Create(TheASTContext, kontekst,
                           SourceLocation(), SourceLocation(),
                           &TheASTContext.Idents.getOwn(nadjiIme(ime)),
                           tip, nullptr, SC_None);
}

/* Pravljenje izraza deklaracije */
DeclRefExpr *MTKContext::napraviDeclExpr(ValueDecl *dekl) const {
    return DeclRefExpr::Create(TheASTContext, NestedNameSpecifierLoc(),
                               SourceLocation(), dekl, true,
                               SourceLocation(), dekl->getType(), VK_LValue);
}

/* Pravljenje izraza deklaracije */
DeclRefExpr *MTKContext::napraviDeclExpr(DeclStmt *deknar) const {
    return napraviDeclExpr(cast<VarDecl>(deknar->getSingleDecl()));
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

/* Pravljenje naredbe deklaracije */
DeclStmt *MTKContext::napraviDeclStmt(VarDecl *var) const {
    return naHip(DeclStmt(DeclGroupRef(var), SourceLocation(), SourceLocation()));
}

/* Pravljenje naredbe deklaracije */
DeclStmt *MTKContext::napraviDeclStmt(Decl *deklaracija,
                                      const std::string &ime,
                                      const QualType &tip,
                                      Expr *pocetna) const {
    /* Deklaracija nove promenljive */
    auto var = napraviVar(deklaracija->getDeclContext(), tip, ime);

    /* Celobrojna vrednost za inicijalizaciju */
    var->setInit(pocetna);

    /* Naredba deklaracije uslovne promenljive */
    return napraviDeclStmt(var);
}

/* Pravljenje deklaracije uslovne promenljive */
DeclStmt *MTKContext::napraviUslovnu(Decl *deklaracija,
                                     const std::string &ime,
                                     const bool pocetna) const {
    return napraviDeclStmt(deklaracija, ime,
                           TheASTContext.IntTy,
                           napraviInt(pocetna));
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

/* Pravljenje referenciranja */
UnaryOperator *MTKContext::napraviRef(DeclRefExpr *var) const {
    return napraviUnarni(var, UO_AddrOf, TheASTContext.getPointerType(var->getType()));
}

/* Pravljenje dereferenciranja */
ParenExpr *MTKContext::napraviDeref(DeclRefExpr *var) const {
    return napraviParen(napraviUnarni(var, UO_Deref, var->getType()->getPointeeType()));
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
    const auto boolean = izraz->isKnownToHaveBooleanValue();

    /* Istinitosni tip se pretvara u ceo broj */
    if (boolean) return dohvatiCelobrojnu(izraz);
    /* Celobrojni neistinitosni tip se ne menja */
    else if (integer) return izraz;
    /* Necelobrojni tip se duplo negira i kastuje */
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

/* Pravljenje funkcije */
FunctionDecl *MTKContext::napraviFunkciju(DeclContext *kontekst,
                                          const QualType &tip,
                                          const std::string &ime) const {
    return FunctionDecl::Create(TheASTContext, kontekst,
                                SourceLocation(), SourceLocation(),
                                &TheASTContext.Idents.getOwn(nadjiIme(ime)),
                                tip, nullptr, SC_None);
}

/* Pravljenje parametra */
ParmVarDecl *MTKContext::napraviParam(VarDecl *var) const {
    return ParmVarDecl::Create(TheASTContext, var->getDeclContext(),
                           SourceLocation(), SourceLocation(),
                           var->getIdentifier(), var->getType(),
                           nullptr, SC_None, nullptr);
}

/* Pravljenje funkcije */
FunctionDecl *MTKContext::napraviFunkciju(DeclContext *kontekst,
                                          const QualType &tipRet,
                                          const std::string &ime,
                                          const std::vector<VarDecl *> &parms,
                                          Stmt *telo) const {
    /* Izracunavanje parametara */
    std::vector<ParmVarDecl *> params;
    std::transform(std::cbegin(parms),
                   std::cend(parms),
                   std::back_inserter(params),
                   [this](VarDecl *const var) {
                       return napraviParam(var);
                   });

    /* Izracunavanje tipa parametara */
    std::vector<QualType> tipParams;
    std::transform(std::cbegin(params),
                   std::cend(params),
                   std::back_inserter(tipParams),
                   [](ParmVarDecl *const var) {
                       return var->getType();
                   });

    /* Izracunavanje tipa funkcije */
    const auto tip = TheASTContext.getFunctionType(
                         tipRet, tipParams,
                         FunctionProtoType::ExtProtoInfo()
                     );

    /* Popunjavanje zeljene funkcije */
    const auto fja = napraviFunkciju(kontekst, tip, ime);
    fja->setParams(params); fja->setBody(telo); return fja;
}

/* Pravljenje ref funkcije */
FunctionDecl *MTKContext::napraviRefFunkciju(DeclContext *kontekst,
                                             const QualType &tipRet,
                                             const std::string &ime,
                                             const std::vector<VarDecl *> &parms,
                                             Stmt *telo) const {
    /* Izracunavanje parametara */
    std::for_each(std::cbegin(parms),
                  std::cend(parms),
                  [this](VarDecl *const var) {
                      var->setType(TheASTContext.getPointerType(var->getType()));
                  });

    /* Popunjavanje zeljene funkcije */
    return napraviFunkciju(kontekst, tipRet, ime, parms, telo);
}

/* Pravljenje poziva */
CallExpr *MTKContext::napraviPoziv(FunctionDecl *funkcija,
                                   const std::vector<Expr *> &args) const {
    return CallExpr::Create(TheASTContext, napraviDeclExpr(funkcija),
                            args, funkcija->getReturnType(),
                            VK_RValue, SourceLocation());
}

/* Pravljenje ref poziva */
CallExpr *MTKContext::napraviRefPoziv(FunctionDecl *funkcija,
                                      const std::vector<DeclRefExpr *> &args) const {
    /* Izracunavanje parametara */
    std::vector<Expr *> argums;
    std::transform(std::cbegin(args),
                   std::cend(args),
                   std::back_inserter(argums),
                   [this](DeclRefExpr *const var) {
                       return napraviRef(var);
                   });

    /* Popunjavanje zeljene funkcije */
    return napraviPoziv(funkcija, argums);
}

/* Pravljenje ref poziva */
CallExpr *MTKContext::napraviRefPoziv(FunctionDecl *funkcija,
                                      const std::vector<VarDecl *> &args) const {
    /* Izracunavanje parametara */
    std::vector<DeclRefExpr *> argums;
    std::transform(std::cbegin(args),
                   std::cend(args),
                   std::back_inserter(argums),
                   [this](VarDecl *const var) {
                       return napraviDeclExpr(var);
                   });

    /* Popunjavanje zeljene funkcije */
    return napraviRefPoziv(funkcija, argums);
}

/* Pravljenje return naredbe */
ReturnStmt *MTKContext::napraviReturn(Expr *izraz) const {
    return ReturnStmt::Create(TheASTContext, SourceLocation(), izraz, nullptr);
}

/* Pravljenje return naredbe */
ReturnStmt *MTKContext::napraviReturn(bool vrednost) const {
    return napraviReturn(napraviInt(vrednost));
}

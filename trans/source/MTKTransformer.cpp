#include "While2DoVisitor.hpp"
#include "Do2ForVisitor.hpp"
#include "While2ForVisitor.hpp"
#include "For2WhileVisitor.hpp"
#include "For2DoVisitor.hpp"
#include "PrepForVisitor.hpp"
#include "LoopUnrollVisitor.hpp"
#include "Rek2IterVisitor.hpp"
#include "FinIterVisitor.hpp"
#include "Iter2RekVisitor.hpp"
#include "FinRekVisitor.hpp"
#include "PrepIfVisitor.hpp"
#include "PrepIffVisitor.hpp"
#include "PrepSwitchVisitor.hpp"
#include "If2SwitchVisitor.hpp"
#include "Switch2IfVisitor.hpp"
#include "CodeImputVisitor.hpp"
#include "For2GotoVisitor.hpp"
#include "While2GotoVisitor.hpp"
#include "Do2GotoVisitor.h"
#include "MTKTransformer.hpp"
#include "PrepWhile2Goto.hpp"
#include "PrepFor2Goto.hpp"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Parse/ParseAST.h"
#include "llvm/Support/Host.h"

#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>
/* Podatak o uocenoj gresci */
bool MTKTransformer::error = false;

/* Inicijalizacija transformatora */
MTKTransformer::MTKTransformer(const std::string &s,
                               const std::string &n)
    : stara(s), nova(n) {
    /* Nije sigurno da bude ista datoteka */
    if (stara == nova) {
        greska(istaDatoteka);
        return;
    }

    /* Stara mora da bude validan kod */
    proveri();

    /* Nova ne sme da postoji vec */
    std::ifstream ulaz(nova);
    if (ulaz) {
        greska(postojiDatoteka);
        return;
    }

    /* Postavljanje skupa funkcija */
    MTKContext::postaviFje(fje);

    /* Postavljanje izlazne datoteke */
    MTKContext::postaviDat(nova);

    /* Inicijalizacija instance prevodioca */
    TheCompInst.createDiagnostics();
    auto &TheDiagnostics = TheCompInst.getDiagnostics();
    TheDiagnostics.setSuppressAllDiagnostics(true);

    /* Postavljanje podataka o ciljnoj platformi */
    auto TO = std::make_shared<TargetOptions>();
    TO->Triple = llvm::sys::getDefaultTargetTriple();
    auto TI = TargetInfo::CreateTargetInfo(TheDiagnostics, TO);
    TheCompInst.setTarget(TI);
}

/* Prijavljivanje greske u radu */
int MTKTransformer::greska(const std::string &poruka, bool fatal) {
    error = true;
    return MTKContext::greska(poruka, fatal);
}

/* Resetovanje statickih parametara */
void MTKTransformer::resetuj() {
    error = false;
    PrepForVisitor::prviProlaz = true;
    FinRekVisitor::prviProlaz = true;
    PrepForVisitor::posaoBroj = 0;
    FinRekVisitor::posaoBroj = 0;
}

/* Obrada zeljene transformacije */
int MTKTransformer::obradi(int argc, const char *argv[]) {
    /* Prekid pogresno pokrenutog programa */
    if (argc != 4) return greska(upotreba);

    /* Citanje argumenata */
    std::string stara(argv[1]);
    std::string nova(argv[2]);
    std::string radnja(argv[3]);

    /* Regularni izrazi za parametre */
    const std::regex ro("^o(\\d+)$"),
                     ru("^u(\\d+)$");
    std::smatch pogodak;

    /* Instanciranje transformatora */
    MTKTransformer trans(stara, nova);

    /* Prvi deo algoritma */
    if (radnja == "do") {
        trans.primeni(Izmena::While2Do);
        trans.primeni(Izmena::PrepFor);
        trans.primeni(Izmena::For2Do);
    } else if (radnja == "for") {
        trans.primeni(Izmena::Do2For);
        trans.primeni(Izmena::While2For);
    } else if (radnja == "while") {
        trans.primeni(Izmena::Do2For);
        trans.primeni(Izmena::PrepFor);
        trans.primeni(Izmena::For2While);
    } else if (radnja == "if") {
        trans.primeni(Izmena::PrepSwitch);
        trans.primeni(Izmena::Switch2If);
    } else if (radnja == "switch") {
        trans.primeni(Izmena::PrepIff);
        trans.primeni(Izmena::PrepIf);
        trans.primeni(Izmena::If2Switch);
    } else if (radnja == "iter") {
        trans.primeni(Izmena::Rek2Iter);
        trans.primeni(Izmena::FinIter);
    } else if (radnja == "rek") {
        trans.primeni(Izmena::Do2For);
        trans.primeni(Izmena::PrepFor);
        trans.primeni(Izmena::For2While);
        trans.primeni(Izmena::Iter2Rek);
        trans.primeni(Izmena::FinRek);
    } else if (std::regex_match(radnja, pogodak, ro)) {
        postaviOdmotavanje(std::stoull(pogodak[1].str()));
        trans.primeni(Izmena::LoopUnroll);
    } else if (std::regex_match(radnja, pogodak, ru)) {
        postaviVerovatnocu(std::stoull(pogodak[1].str()));
        trans.primeni(Izmena::CodeImput);
    } else if (radnja == "goto") {
        trans.primeni(Izmena::PrepWhile2Goto);
        trans.primeni(Izmena::PrepFor2Goto);

        trans.primeni(Izmena::For2Goto);
        trans.primeni(Izmena::While2Goto);
    /* Prekid pogresno pokrenutog programa */
    } else return greska(upotreba);

    /* Izvestavanje o uocenoj gresci */
    if (error) return EXIT_FAILURE;

    /* Lepo formatiranje novog koda */
    std::ostringstream buffer;
    buffer << "clang-format-11 -i " << nova;
    std::system(buffer.str().c_str());

    /* Normalno zavrsavanje programa */
    return EXIT_SUCCESS;
}

/* Odabir odgovarajuceg transformatora */
ASTConsumer *MTKTransformer::odaberiTransformator(Izmena izmena) {
    switch (izmena) {
    case Izmena::While2Do: return new MTKConsumer<While2DoVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::Do2For: return new MTKConsumer<Do2ForVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::While2For: return new MTKConsumer<While2ForVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::PrepFor: return new MTKConsumer<PrepForVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::For2While: return new MTKConsumer<For2WhileVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::For2Do: return new MTKConsumer<For2DoVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::LoopUnroll: return new MTKConsumer<LoopUnrollVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::Rek2Iter: return new MTKConsumer<Rek2IterVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::FinIter: return new MTKConsumer<FinIterVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::Iter2Rek: return new MTKConsumer<Iter2RekVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::FinRek: return new MTKConsumer<FinRekVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::PrepIf: return new MTKConsumer<PrepIfVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::PrepIff: return new MTKConsumer<PrepIffVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::PrepSwitch: return new MTKConsumer<PrepSwitchVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::If2Switch: return new MTKConsumer<If2SwitchVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::Switch2If: return new MTKConsumer<Switch2IfVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::CodeImput: return new MTKConsumer<CodeImputVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::While2Goto: return new MTKConsumer<While2GotoVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::For2Goto: return new MTKConsumer<For2GotoVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::Do2Goto: return new MTKConsumer<Do2GotoVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::PrepFor2Goto: return new MTKConsumer<PrepFor2GotoVisitor>(*TheRewriter, *TheASTContext);
    case Izmena::PrepWhile2Goto: return new MTKConsumer<PrepWhile2GotoVisitor>(*TheRewriter, *TheASTContext);
    }
}

/* Potencijalne izmene koda */
bool MTKTransformer::izmeniKod(Izmena izmena) {
    /* Upisivanje novog koda iz bafera; u slucaju da nema
     * izmena, prosto prepisivanje starog koda kao novog */
    const auto RewriteBuf =
            TheRewriter->getRewriteBufferFor(TheCompInst
                                             .getSourceManager()
                                             .getMainFileID());
    if (RewriteBuf) {
        /* Otvaranje izlazne datoteke */
        std::ofstream izlaz(nova);
        if (!izlaz) return greska(nemaDatoteke);

        /* Upisivanje rezultata */
        izlaz << std::string(RewriteBuf->begin(), RewriteBuf->end());
    } else if (stara == nova) {
        if ((izmena != Izmena::FinRek ||
             FinRekVisitor::imaloPosla()) &&
             izmena != Izmena::PrepFor) return true;
    } else {
        /* Otvaranje ulazne i izlazne datoteke */
        std::ifstream ulaz(stara);
        std::ofstream izlaz(nova);
        if (!ulaz || !izlaz) return greska(nemaDatoteke);

        /* Prepisivanje ulaza na izlaz */
        std::ostringstream buffer;
        buffer << ulaz.rdbuf();
        izlaz << buffer.str();

        /* Iskakanje ako treba */
        if ((izmena != Izmena::FinRek ||
             FinRekVisitor::imaloPosla()) &&
             izmena != Izmena::PrepFor) return true;
    }

    /* Inace ne treba iskociti */
    return false;
}

/* Provera validnosti ulaza */
void MTKTransformer::proveri() const {
    /* Pokusaj prevodjenja ulaza */
    std::ostringstream buffer;
    buffer << "clang-11 -fsyntax-only " << stara << " 2>/dev/null";
    const auto ret
        = std::system(buffer.str().c_str());

    /* Prijava greske ako nesto nije u redu */
    if (!WIFEXITED(ret) || WEXITSTATUS(ret)) greska(losUlaz, true);
}

/* Primena zeljene izmene koda; sustinski je
 * boilerplate (sablonski) kod za rad sa AST */
void MTKTransformer::primeni(Izmena izmena) {
    /* Odustajanje u slucaju greske */
    if (error) return;

    /* Parsiranje dokle god ima promena */
    for(;;) {
        /* Pravljenje upravljaca datoteka */
        TheCompInst.createFileManager();
        auto &FileMgr = TheCompInst.getFileManager();

        /* Pravljenje upravljaca izvora */
        TheCompInst.createSourceManager(FileMgr);
        auto &SourceMgr = TheCompInst.getSourceManager();

        /* Pravljenje pretprocesora */
        TheCompInst.createPreprocessor(TU_Module);
        auto &ThePreprocessor = TheCompInst.getPreprocessor();

        /* Pravljenje AST konteksta */
        TheCompInst.createASTContext();

        /* Inicijalizacija prepisivaca koda */
        Rewriter TheRewriterr; TheRewriter = &TheRewriterr;
        TheRewriter->setSourceMgr(SourceMgr, TheCompInst.getLangOpts());

        /* Citanje prosledjenog fajla */
        const auto FileInOpt = FileMgr.getFile(stara);
        if (!FileInOpt) { greska(nemaDatoteke); return; }
        const auto FileIn = FileInOpt.get();

        /* Postavljanje prosledjenog fajla za ulazni */
        SourceMgr.setMainFileID(
                    SourceMgr.createFileID(FileIn, SourceLocation(), SrcMgr::C_User));
        TheCompInst.getDiagnosticClient().BeginSourceFile(
                    TheCompInst.getLangOpts(), &ThePreprocessor);

        /* Odabir transformatora prema izmeni */
        TheASTContext = &TheCompInst.getASTContext();
        auto TheConsumer = odaberiTransformator(izmena);

        /* Parsiranje i obrada AST stabla */
        ParseAST(ThePreprocessor, TheConsumer, *TheASTContext);
        delete TheConsumer;

        /* Potencijalne izmene koda */
        if (izmeniKod(izmena)) return;

        /* Zamena starog fajla */
        stara = nova;

        /* Provera da li je sve u redu */
        if (izmena == Izmena::Iter2Rek)
            proveri();

        /* Iteracije i pripreme su jednoprolazni */
        if ((izmena == Izmena::PrepFor &&
             PrepForVisitor::imaloPosla()) ||
            izmena == Izmena::LoopUnroll ||
            izmena == Izmena::PrepIf ||
            izmena == Izmena::PrepSwitch ||
            izmena == Izmena::If2Switch ||
            izmena == Izmena::Rek2Iter ||
            izmena == Izmena::FinIter ||
            izmena == Izmena::CodeImput ||
            izmena == Izmena::PrepWhile2Goto ||
            izmena == Izmena::PrepFor2Goto

            )
            return;
    }
}

/* Registrovanje zeljenog broja odmotavanja */
void MTKTransformer::postaviOdmotavanje(unsigned long long n) {
    LoopUnrollVisitor::postaviBroj(n);
}

/* Registrovanje zeljenog broja umetanja */
void MTKTransformer::postaviVerovatnocu(unsigned long long n) {
    CodeImputVisitor::postaviBroj(n);
}

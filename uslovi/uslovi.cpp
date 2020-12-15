#include "PrepIfVisitor.hpp"
#include "PrepSwitchVisitor.hpp"
#include "If2SwitchVisitor.hpp"
#include "Switch2IfVisitor.hpp"

#include "clang/Basic/FileManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Parse/ParseAST.h"
#include "llvm/Support/Host.h"

#include <fstream>
#include <sstream>

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
const auto greska = "Neuspelo otvaranje datoteke!\n";

/* Obrada prema zeljenoj akciji; sustinski
 * boilerplate (sablonski) kod za rad sa AST */
static void obradi(const Akcija &akcija) {
    /* Parsiranje dokle god ima promena */
    for(;;) {
        /* Pravljenje i inicijalizacija prevodioca */
        CompilerInstance TheCompInst;
        TheCompInst.createDiagnostics();
        auto &TheDiagnostics = TheCompInst.getDiagnostics();
        TheDiagnostics.setSuppressAllDiagnostics(true);

        /* Postavljanje podataka o ciljnoj platformi */
        auto TO = std::make_shared<TargetOptions>();
        TO->Triple = llvm::sys::getDefaultTargetTriple();
        auto TI = TargetInfo::CreateTargetInfo(TheDiagnostics, TO);
        TheCompInst.setTarget(TI);

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

        /* Pravljenje prepisivaca koda */
        Rewriter TheRewriter;
        TheRewriter.setSourceMgr(SourceMgr, TheCompInst.getLangOpts());

        /* Citanje prosledjenog fajla */
        const auto FileInOpt = FileMgr.getFile(*stari);
        if (!FileInOpt) {
            llvm::errs() << greska;
            exit(EXIT_FAILURE);
        }
        const auto FileIn = FileInOpt.get();

        /* Postavljanje prosledjenog fajla za ulazni */
        SourceMgr.setMainFileID(
            SourceMgr.createFileID(FileIn, SourceLocation(), SrcMgr::C_User));
        TheCompInst.getDiagnosticClient().BeginSourceFile(
            TheCompInst.getLangOpts(), &ThePreprocessor);

        /* Odabir petlje u koju se ostale menjaju */
        auto &TheASTContext = TheCompInst.getASTContext();
        ASTConsumer* TheConsumer;
        switch (akcija) {
            case Akcija::PrepIf:
                TheConsumer = new MTKConsumer<PrepIfVisitor>(TheRewriter, TheASTContext);
                break;
            case Akcija::PrepSwitch:
                TheConsumer = new MTKConsumer<PrepSwitchVisitor>(TheRewriter, TheASTContext);
                break;
            case Akcija::If2Switch:
                TheConsumer = new MTKConsumer<If2SwitchVisitor>(TheRewriter, TheASTContext);
                break;
            case Akcija::Switch2If:
                TheConsumer = new MTKConsumer<Switch2IfVisitor>(TheRewriter, TheASTContext);
                break;
        }

        /* Parsiranje i obrada AST stabla */
        ParseAST(ThePreprocessor, TheConsumer, TheASTContext);
        delete TheConsumer;

        /* Upisivanje novog koda iz bafera; u slucaju da nema
         * izmena, prosto prepisivanje starog koda kao novog */
        const auto *RewriteBuf =
            TheRewriter.getRewriteBufferFor(SourceMgr.getMainFileID());
        if (RewriteBuf) {
            /* Otvaranje izlazne datoteke */
            std::ofstream izlaz(*novi);
            if (!izlaz) {
                llvm::errs() << greska;
                exit(EXIT_FAILURE);
            }

            /* Upisivanje rezultata */
            izlaz << std::string(RewriteBuf->begin(), RewriteBuf->end());
        } else if (*stari == *novi) {
            break;
        } else {
            /* Otvaranje ulazne i izlazne datoteke */
            std::ifstream ulaz(*stari);
            std::ofstream izlaz(*novi);
            if (!ulaz || !izlaz) {
                llvm::errs() << greska;
                exit(EXIT_FAILURE);
            }

            /* Prepisivanje ulaza na izlaz */
            std::ostringstream buffer;
            buffer << ulaz.rdbuf();
            izlaz << buffer.str();
            break;
        }

        /* Zamena starog fajla */
        *stari = *novi;

        /* Priprema je jednoprolazna */
        if (akcija == Akcija::PrepIf || akcija == Akcija::PrepSwitch)
            break;
    }
}

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

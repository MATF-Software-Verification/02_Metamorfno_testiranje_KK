/* Ukljucivanje implementiranih klasa */
#include "While2DoConsumer.hpp"
#include "Do2ForConsumer.hpp"
#include "While2ForConsumer.hpp"
#include "For2WhileConsumer.hpp"
#include "For2DoConsumer.hpp"
#include "PrepForConsumer.hpp"

/* Enumeracija akcija */
enum class Akcija {
    While2Do,
    Do2For,
    While2For,
    PrepFor,
    For2While,
    For2Do
};

/* Nacin upotrebe programa */
static std::string stari, novi, petlja;
const auto upotreba = "Upotreba: ./petlje <ulaz> <izlaz> <do|while|for>\n";
const auto greska = "Neuspelo otvaranje datoteke!\n";

/* Obrada prema zeljenoj akciji; sustinski
 * boilerplate (sablonski) kod za rad sa AST */
static void obradi(const Akcija akcija) {
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
        const auto FileIn = FileMgr.getFile(stari).get();
        if (!FileIn) {
            llvm::errs() << greska;
            exit(EXIT_FAILURE);
        }

        /* Postavljanje prosledjenog fajla za ulazni */
        SourceMgr.setMainFileID(
            SourceMgr.createFileID(FileIn, SourceLocation(), SrcMgr::C_User));
        TheCompInst.getDiagnosticClient().BeginSourceFile(
            TheCompInst.getLangOpts(), &ThePreprocessor);

        /* Odabir petlje u koju se ostale menjaju */
        auto &TheASTContext = TheCompInst.getASTContext();
        ASTConsumer* TheConsumer;
        switch (akcija) {
            case Akcija::While2Do:
                TheConsumer = new LoopConsumer<While2DoVisitor>(TheRewriter, TheASTContext);
                break;
            case Akcija::Do2For:
                TheConsumer = new LoopConsumer<Do2ForVisitor>(TheRewriter, TheASTContext);
                break;
            case Akcija::While2For:
                TheConsumer = new LoopConsumer<While2ForVisitor>(TheRewriter, TheASTContext);
                break;
            case Akcija::PrepFor:
                TheConsumer = new LoopConsumer<PrepForVisitor>(TheRewriter, TheASTContext);
                break;
            case Akcija::For2While:
                TheConsumer = new LoopConsumer<For2WhileVisitor>(TheRewriter, TheASTContext);
                break;
            case Akcija::For2Do:
                TheConsumer = new LoopConsumer<For2DoVisitor>(TheRewriter, TheASTContext);
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
            std::ofstream izlaz(novi);
            if (!izlaz) {
                llvm::errs() << greska;
                exit(EXIT_FAILURE);
            }

            /* Upisivanje rezultata */
            izlaz << std::string(RewriteBuf->begin(), RewriteBuf->end());
        } else if (stari == novi) {
            break;
        } else {
            /* Otvaranje ulazne i izlazne datoteke */
            std::ifstream ulaz(stari);
            std::ofstream izlaz(novi);
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
        stari = novi;

        /* Priprema je jednoprolazna */
        if (akcija == Akcija::PrepFor)
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
    stari = argv[1];
    novi = argv[2];
    petlja = argv[3];

    /* Prekid pogresno pokrenutog programa */
    if (petlja != "do" &&
        petlja != "while" &&
        petlja != "for") {
        llvm::errs() << upotreba;
        exit(EXIT_FAILURE);
    }

    /* Prvi deo algoritma */
    if (petlja == "do")
        obradi(Akcija::While2Do);
    else
        obradi(Akcija::Do2For);

    /* Drugi deo algoritma */
    if (petlja == "for")
        obradi(Akcija::While2For);
    else
        obradi(Akcija::PrepFor);

    /* Treci deo algoritma */
    if (petlja == "while")
        obradi(Akcija::For2While);
    else if (petlja == "do")
        obradi(Akcija::For2Do);

    /* Lepo formatiranje novog koda */
    std::ostringstream buffer;
    buffer << "clang-format -i " << novi;
    std::system(buffer.str().c_str());

    /* Normalno zavrsavanje programa */
    exit(EXIT_SUCCESS);
}

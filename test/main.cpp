#include <QtTest>

#include <unistd.h>

#include "MTKTransformer.hpp"

/* Klasa za testiranje transformatora */
class MTKTest : public QObject {
    Q_OBJECT

/* Pozicioniranje u konstruktoru */
public:
    MTKTest();

/* Pomocne funkcije za provere */
private:
    void prevedi(const std::string &,
                 const std::string &);
    void akcija(const std::string &,
                const std::string &,
                const std::string &);

/* Slotovi za konkretne testove */
private Q_SLOTS:
    void petlje();
};

/* Pozicioniranje u konstruktoru */
MTKTest::MTKTest() {
    chdir("../test");
}

/* Prevodjenje zadatog programa */
void MTKTest::prevedi(const std::string &in,
                      const std::string &out) {
    const auto prev = "clang-11 -O0 " + in;
    std::system(prev.c_str());

    /* Cuvanje izlaza zadatog programa */
    const auto sav = "./a.out > " + out;
    std::system(sav.c_str());

    /* Brisanje izvrsive verzije programa */
    std::system("rm a.out");
}

/* Primena transformatora */
void MTKTest::akcija(const std::string &in,
                     const std::string &out,
                     const std::string &op) {
    /* Ciscenje izlaznog programa */
    const auto cist = "rm " + out + " 2> /dev/null";
    std::system(cist.c_str());

    /* Resetovanje statickih parametara */
    MTKTransformer::resetuj();

    /* Pravljenje izlaznog programa */
    const char *argv[] = {"trans", in.c_str(),
                          out.c_str(), op.c_str()};
    const int argc = sizeof argv / sizeof *argv;
    QVERIFY(!MTKTransformer::obradi(argc, argv));

    /* Prevodjenje izlaznog programa */
    prevedi(out, "output2.txt");

    /* Provera razlike izlaza */
    const auto rez =
        std::system("diff output.txt output2.txt -q");
    QVERIFY(WIFEXITED(rez) && !WEXITSTATUS(rez));

    /* Ciscenje izlaza programa */
    std::system("rm output2.txt");
}

/* Provera rada sa petljama */
void MTKTest::petlje() {
    chdir("petlje");

    /* Prevodjenje ulaznog programa */
    prevedi("whilefor.c", "output.txt");

    /* Prva iteracija tranformacija */
    akcija("whilefor.c", "samodo.c", "do");
    akcija("whilefor.c", "samofor.c", "for");
    akcija("whilefor.c", "samowhile.c", "while");

    /* Druga iteracija transformacija */
    akcija("samodo.c", "samodo2.c", "do");
    akcija("samodo.c", "samofor2.c", "for");
    akcija("samodo.c", "samowhile2.c", "while");

    /* Vracanje na pocetnu poziciju */
    chdir("..");
}

/* Automatski main sa jednom klasom */
QTEST_APPLESS_MAIN(MTKTest)

/* Moc sa svim potrebnim informacijama */
#include "main.moc"

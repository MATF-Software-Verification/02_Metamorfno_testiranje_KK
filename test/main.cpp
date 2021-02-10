#include <QtTest>

class MTKTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test();
};

void MTKTest::test()
{
    QCOMPARE(0, 0);
}

QTEST_APPLESS_MAIN(MTKTest)

#include "main.moc"

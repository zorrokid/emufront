#include "platformtest.h"


void PlatformTest::init()
{
}

void PlatformTest::initTestCase()
{
}

void PlatformTest::cleanup()
{
}

void PlatformTest::cleanupTestCase()
{
}

/*void PlatformTest::equals_data()
{
    qDebug() << "Arranging data for equals.";

    QTest::addColumn<Platform>("platform1");
    QTest::addColumn<Platform>("platform2");
    qDebug() << "Done columns";
    efA = new EmuFrontFile(1, "a", "qa", 1, 2);
    efB = new EmuFrontFile(2, "b", "qaa", 2, 3);
    qDebug() << "efA" << efA->getName();
    qDebug() << "efB" << efB->getName();

    QTest::newRow("id and name")
            << Platform(1, "test", efA)
            << Platform(1, "test", efB);

    qDebug() << "Done first row";
    efA = new EmuFrontFile(1, "a", "qa", 1, 2);
    efB = new EmuFrontFile(2, "b", "qaa", 2, 3);
    qDebug() << "efA" << efA->getName();
    qDebug() << "efB" << efB->getName();

    QTest::newRow("id, name and filename")
            << Platform(2, "test", efA)
            << Platform(2, "test", efB);
    qDebug() << "Done 2nd row";
}

void PlatformTest::equals()
{
    qDebug() << "Entering equals";
    QFETCH(Platform, platform1);
    QFETCH(Platform, platform2);
    QVERIFY(platform1 == platform2);
    qDebug() << "Leaving equals";
}*/

void PlatformTest::equals2()
{
    qDebug() << "Starting equals2 test, creating EmuFrontFile objects to heap.";
    EmuFrontFile *efA = new EmuFrontFile(1, "efA", "qa", 1, 2);
    EmuFrontFile *efB = new EmuFrontFile(2, "efB", "qaa", 2, 3);
    qDebug() << "Creating Platform test objects p1 and p2";
    Platform p1(1, "px");
    Platform p2(1, "px");
    QVERIFY(p1 == p2);

    qDebug() << efB->getName();

    qDebug() << "Creating Platform test objects p5 containg member " << efA->getName();
    Platform p5(1, "p5", efA);

    // The following should not be done, efA dies with p5:
    // and pointer from p6 would keep pointing to memory area where
    // efA no longer exists:
    //Platform p6(1, "p6", efA);

    qDebug() << "Creating Platform test objects p6 containg member " << efB->getName();
    Platform p6(1, "p6", efB);

    qDebug() << "efA" << efA->getName();
    qDebug() << "efB" << efB->getName();

    qDebug() << "Entering QVERIFY";
    QVERIFY(p5 == p6);

    qDebug() << "efA" << efA->getName();
    qDebug() << "efB" << efB->getName();

    QVERIFY(p5 == p6);

    qDebug() << "efA" << efA->getName();
    qDebug() << "efB" << efB->getName();

    qDebug() << "Leaving QVERIFY";
    qDebug() << "Leaving equals2";
}

/* Platforms are equal if the following fields match:
    - id (int)
    - name (QString)
    - filename (QString)
*/
void PlatformTest::notEquals()
{
    Platform p1(1, "testa");
    Platform p2(1, "test");
    // This should return true
    QVERIFY(p1 != p2);
}

#include "platformtest.h"


void PlatformTest::init()
{
    qDebug() << "Creating efA and efB.";
    //efA = new EmuFrontFile(1, "a", "qa", 1, 2);
    //efB = new EmuFrontFile(2, "b", "qaa", 2, 3);
}

void PlatformTest::initTestCase()
{
    qDebug() << "Initializing PlatformTest.";
}

void PlatformTest::cleanup()
{
    qDebug() << "cleanup";
    // The following objects have already been deleted
    //delete efA;
    //delete efB;
    //efA = 0;
    //efB = 0;
}

void PlatformTest::cleanupTestCase()
{
    qDebug() << "Cleaning up PlatformTest.";
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
    qDebug() << "Starting equals2";
    EmuFrontFile *efA = new EmuFrontFile(1, "a", "qa", 1, 2);
    EmuFrontFile *efB = new EmuFrontFile(2, "b", "qaa", 2, 3);
    Platform p1(1, "test");
    Platform p2(1, "test");
    QVERIFY(p1 == p2);

    qDebug() << "efA" << efA->getName();
    qDebug() << "efB" << efB->getName();

    Platform p5(1, "test", efA);
    // The following cannot be done, efA dies with p5:
    // and pointer from p6 would keep pointing to memory area where
    // efA no longer exists:
    //Platform p6(1, "test", efA);
    Platform p6(1, "test", efB);

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
    efA->deleteLater();
    efB->deleteLater();
}

/* Platforms are equal if the following fields match:
    - id (int)
    - name (QString)
    - filename (QString)
*/
/*void PlatformTest::notEquals()
{
    Platform p1(1, "testa");
    Platform p2(1, "test");
    // This should return true
    QVERIFY(p1 != p2);
}*/

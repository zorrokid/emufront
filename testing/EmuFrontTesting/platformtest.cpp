#include "platformtest.h"
#include "../../src/dataobjects/platform.h"
#include "../../src/dataobjects/emufrontfile.h"

Q_DECLARE_METATYPE(EmuFrontFile)
Q_DECLARE_METATYPE(Platform)

void PlatformTest::initTestCase()
{
    qDebug() << "Initializing PlatformTest.";
    EmuFrontFile *efile = new EmuFrontFile(1, "zzz.png", "2hxxxx", 2, 1);
}

void PlatformTest::cleanupTestCase()
{
    qDebug() << "Cleaning up PlatformTest.";
    delete efile;
}

void PlatformTest::equals_data()
{
    QTest::addColumn<Platform>("platform1");
    QTest::addColumn<Platform>("platform2");
    QTest::newRow("id and name")
        << Platform(1, "test", efile)
        << Platform(1, "test", efile);
    QTest::newRow("id, name and filename")
        << Platform(2, "test", efile)
        << Platform(2, "test", efile);
}

void PlatformTest::equals()
{
    QFETCH(Platform, platform1);
    QFETCH(Platform, platform2);
    QVERIFY(platform1 == platform2);
}

/* Platforms are equal if the following fields match:
    - id (int)
    - name (QString)
*/
void PlatformTest::notEquals()
{
    Platform p1(1, "testa", efile);
    Platform p2(1, "test", efile);
    // This should return true
    QVERIFY(p1 != p2);
}

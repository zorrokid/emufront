#include "mediatypetest.h"

void MediaTypeTest::equals_data()
{
    qDebug() << "equals_data() starting.";
    QTest::addColumn<MediaType>("mt1");
    QTest::addColumn<MediaType>("mt2");

    qDebug() << "Adding row 1 with id and name parameters.";

    QTest::newRow("id and name")
        << MediaType(1, "Cartridge")
        << MediaType(1, "Cartridge");

    qDebug() << "Adding row 2 with id, name and EmuFrontFile object";

    QTest::newRow("id, name and EmuFrontFile object")
        << MediaType(2, "Disk", new EmuFrontFile(1, "test", "000", 1, 1))
        << MediaType(2, "Disk", new EmuFrontFile(1, "test", "000", 1, 1));

    qDebug() << "equals_data() finishing.";
}

void MediaTypeTest::equals()
{
    qDebug() << "Entering equals";
    QFETCH(MediaType, mt1);
    QFETCH(MediaType, mt2);
    QVERIFY(mt1 == mt2);
    qDebug() << "Leaving equals";
}

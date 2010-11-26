#include "emufrontobjecttest.h"

void EmuFrontObjectTest::constructTest_data()
{
    QTest::addColumn<EmuFrontObject>("mt1");
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("id");
    QTest::newRow("no parameters")
        << EmuFrontObject()
        << "" << EmuFrontObject::ID_NOT_SET;
    QTest::newRow("id and empty string as name")
        << EmuFrontObject(1, "")
        << "" << 1;
    QTest::newRow("id, name")
        << EmuFrontObject(2, "Disk")
        << "Disk" << 2;
}

void EmuFrontObjectTest::constructTest()
{
    QFETCH(EmuFrontObject, mt1);
    QFETCH(QString, name);
    QFETCH(int, id);
    QVERIFY(mt1.getName() == name && mt1.getId() == id);
}

void EmuFrontObjectTest::equals_data()
{
    QTest::addColumn<EmuFrontObject>("mt1");
    QTest::addColumn<EmuFrontObject>("mt2");
    QTest::newRow("no parameters")
        << EmuFrontObject()
        << EmuFrontObject();
    QTest::newRow("id and empty string as name")
        << EmuFrontObject(1, "")
        << EmuFrontObject(1, "");
    QTest::newRow("id, name")
        << EmuFrontObject(2, "Disk")
        << EmuFrontObject(2, "Disk");
}

void EmuFrontObjectTest::equals()
{
    QFETCH(EmuFrontObject, mt1);
    QFETCH(EmuFrontObject, mt2);
    QVERIFY(mt1 == mt2);
}

void EmuFrontObjectTest::notEquals_data()
{
    QTest::addColumn<EmuFrontObject>("mt1");
    QTest::addColumn<EmuFrontObject>("mt2");
    QTest::newRow("1. with no params, 2. with id and name")
        << EmuFrontObject()
        << EmuFrontObject(1, "x");
    QTest::newRow("name differs")
        << EmuFrontObject(1, "")
        << EmuFrontObject(1, "a");
    QTest::newRow("id differs")
        << EmuFrontObject(1, "")
        << EmuFrontObject(-1, "");
    QTest::newRow("id and name differs")
        << EmuFrontObject(3, "Disk")
        << EmuFrontObject(2, "Disak");
}

void EmuFrontObjectTest::notEquals()
{
    QFETCH(EmuFrontObject, mt1);
    QFETCH(EmuFrontObject, mt2);
    QVERIFY(mt1 != mt2);
}

void EmuFrontObjectTest::nameTest()
{
    EmuFrontObject o;
    QString n = "hello";
    o.setName(n);
    QCOMPARE(n, o.getName());
}

void EmuFrontObjectTest::idTest()
{
    EmuFrontObject o;
    int id = 998;
    o.setId(id);
    QCOMPARE(id, o.getId());
}

void EmuFrontObjectTest::copyTest()
{
    EmuFrontObject o(123, "qwerty");
    EmuFrontObject b(321, "ytrewq");
    b = o;
    QVERIFY(o.getName() == b.getName()
        && o.getId() == b.getId()
        && (&o != &b));
}

void EmuFrontObjectTest::copyContructTest()
{
    EmuFrontObject o(123, "qwerty");
    EmuFrontObject b(o);
    QVERIFY(o.getName() == b.getName()
        && o.getId() == b.getId()
        && (&o != &b));
}




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

void EmuFrontObjectTest::nameTest_data()
{
    QTest::addColumn<EmuFrontObject>("efo");
    QTest::addColumn<QString>("name");
    QTest::newRow("default constructor, non empty string")
        << EmuFrontObject()
        << "Hello";
    QTest::newRow("default constructor")
        << EmuFrontObject()
        << "default constructor, empty string";
}

void EmuFrontObjectTest::nameTest()
{
    QFETCH(EmuFrontObject, efo);
    QFETCH(QString, name);
    efo.setName(name);
    QCOMPARE(name, efo.getName());
}

void EmuFrontObjectTest::idTest_data()
{
    QTest::addColumn<EmuFrontObject>("efo");
    QTest::addColumn<int>("id");
    QTest::newRow("default constructor")
        << EmuFrontObject()
        << 998;
    QTest::newRow("default constructor")
        << EmuFrontObject()
        << -12;
}

void EmuFrontObjectTest::idTest()
{
    QFETCH(EmuFrontObject, efo);
    QFETCH(int, id);
    efo.setId(id);
    QCOMPARE(id, efo.getId());
}

void EmuFrontObjectTest::copyTest_data()
{
    QTest::addColumn<EmuFrontObject>("o");
    QTest::addColumn<EmuFrontObject>("b");
    QTest::newRow("no parameters")
        << EmuFrontObject(1, "Test")
        << EmuFrontObject();
    QTest::newRow("id and empty string as name")
        << EmuFrontObject(1, "")
        << EmuFrontObject(2, "zzz");
    QTest::newRow("id, name")
        << EmuFrontObject(123, "Tape")
        << EmuFrontObject(321, "Disk");
}

void EmuFrontObjectTest::copyTest()
{
    QFETCH(EmuFrontObject, o);
    QFETCH(EmuFrontObject, b);
    b = o;
    QVERIFY(o.getName() == b.getName()
        && o.getId() == b.getId()
        && (&o != &b));
}

void EmuFrontObjectTest::copyContructTest_data()
{
    QTest::addColumn<EmuFrontObject>("o");
    QTest::newRow("no parameters")
        << EmuFrontObject();
    QTest::newRow("id and empty string as name")
        << EmuFrontObject(1, "");
    QTest::newRow("id, name")
        << EmuFrontObject(321, "Disk");
}

void EmuFrontObjectTest::copyContructTest()
{
    QFETCH(EmuFrontObject, o);
    EmuFrontObject b(o);
    QVERIFY(o.getName() == b.getName()
        && o.getId() == b.getId()
        && (&o != &b));
}




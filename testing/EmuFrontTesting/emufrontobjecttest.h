#ifndef EMUFRONTOBJECTTEST_H
#define EMUFRONTOBJECTTEST_H

#include <QObject>
#include <QtTest/QtTest>
#include "../../src/dataobjects/emufrontobject.h"

Q_DECLARE_METATYPE(EmuFrontObject)

class EmuFrontObjectTest : public QObject
{
    Q_OBJECT

private slots:
    void notEquals();
    void notEquals_data();
    void equals();
    void equals_data();
    void idTest();
    void idTest_data();
    void nameTest();
    void nameTest_data();
    void copyTest_data();
    void copyTest();
    void copyContructTest_data();
    void copyContructTest();
    void constructTest();
    void constructTest_data();
};

#endif // EMUFRONTOBJECTTEST_H

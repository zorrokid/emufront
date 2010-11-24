#ifndef PLATFORMTEST_H
#define PLATFORMTEST_H

#include <QObject>
#include <QtTest/QtTest>
#include "../../src/dataobjects/platform.h"
#include "../../src/dataobjects/emufrontfile.h"

//Q_DECLARE_METATYPE(EmuFrontFile)
//Q_DECLARE_METATYPE(Platform)

class PlatformTest : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void equals2();
    void notEquals();
    //void equals();
    //void equals_data();
};

#endif // PLATFORMTEST_H

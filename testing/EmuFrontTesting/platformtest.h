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
  //  void equals();
  //  void equals_data();

private:
    //EmuFrontFile *ef;
/*
    EmuFrontFile *efile;*/
    //EmuFrontFile *efA;
    //EmuFrontFile *efB;

};

#endif // PLATFORMTEST_H

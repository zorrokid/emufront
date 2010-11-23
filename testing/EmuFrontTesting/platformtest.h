#ifndef PLATFORMTEST_H
#define PLATFORMTEST_H

#include <QObject>
#include <QtTest/QtTest>

class EmuFrontFile;

class PlatformTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void notEquals();
    void equals();
    void equals_data();

private:
    EmuFrontFile *efile;

};

#endif // PLATFORMTEST_H

#ifndef PLATFORMTEST_H
#define PLATFORMTEST_H

#include <QObject>
#include <QtTest/QtTest>

class PlatformTest : public QObject
{
    Q_OBJECT

private slots:
    void equals();

};

#endif // PLATFORMTEST_H

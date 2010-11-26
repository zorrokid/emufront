#ifndef PLATFORMNAMEDIALOGTEST_H
#define PLATFORMNAMEDIALOGTEST_H

#include <QObject>
#include <QtTest/QtTest>
#include <QtGui>
#include "../../src/dataobjects/platform.h"
#include "../../src/dialogs/platformnamedialog.h"

class PlatformNameDialogTest : public QObject
{
    Q_OBJECT

private slots:
    void testLineEdit();

};

#endif // PLATFORMNAMEDIALOGTEST_H

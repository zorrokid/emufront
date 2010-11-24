#ifndef MEDIATYPETEST_H
#define MEDIATYPETEST_H

#include <QObject>
#include <QtTest/QtTest>
#include "../../src/dataobjects/mediatype.h"
#include "../../src/dataobjects/emufrontfile.h"

Q_DECLARE_METATYPE(EmuFrontFile)
Q_DECLARE_METATYPE(MediaType)

class MediaTypeTest : public QObject
{
    Q_OBJECT

private slots:
    void equals();
    void equals_data();
};

#endif // MEDIATYPETEST_H

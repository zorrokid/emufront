#ifndef DBPLATFORMTEST_H
#define DBPLATFORMTEST_H

#include <QObject>
#include <QtTest/QtTest>
#include "../../src/dataobjects/platform.h"
#include "../../src/db/dbplatform.h"


class DbPlatformTest : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

/* Test the following operations:
    QSqlQueryModel* getDataModel(bool update = false);
    EmuFrontObject* getDataObjectFromModel(QModelIndex*);
    EmuFrontObject* getDataObject(int id);
    EmuFrontObject* getDataObject(QString filter);
    virtual bool updateDataObjectToModel(const EmuFrontObject*);
    virtual int insertDataObjectToModel(const EmuFrontObject*);
    virtual bool deleteDataObjectFromModel(QModelIndex*);
    virtual bool deleteDataObject(int id) const;
    int countDataObjectRefs(int id) const;
    static int getCurrentTimeStamp();
    void resetModel();
    virtual bool updateDataObjectToModel(const EmuFrontObject*);
    int insertDataObjectToModel(const EmuFrontObject*);
    bool deleteDataObjectFromModel(QModelIndex*);
*/

};

#endif // DBPLATFORMTEST_H

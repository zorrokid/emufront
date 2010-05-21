#ifndef DBPLATFORM_H
#define DBPLATFORM_H

#include "databasemanager.h"
#include "../dataobjects/platform.h"

class QModelIndex;

class DbPlatform : public DatabaseManager
{
public:
    DbPlatform(QObject *);
    virtual QSqlTableModel* getDataModel();
    Platform* getPlatformFromModel(QModelIndex);
    bool updatePlatformToModel(const Platform *);
    bool insertPlatformToModel(const Platform *);
    bool deletePlatformFromModel(QModelIndex);

private:
    enum {
        Platform_Id = 0,
        Platform_Name = 1,
        Platform_Filename = 2 };
    static const QString DB_TABLE_NAME_PLATFORM;
    virtual QSqlTableModel* getData();
};

#endif // DBPLATFORM_H

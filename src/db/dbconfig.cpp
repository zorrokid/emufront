#include "dbconfig.h"
#include <QSqlRelationalTableModel>
#include <QSqlQuery>
#include <QSqlRecord>

DbConfig::DbConfig(QObject *parent) :
    QObject(parent)
{
}

QString DbConfig::getTmpDir()
{
    QString r = "";
    QSqlQuery q("SELECT tmpdirpath FROM config");
    if (q.first())
        r = q.value(DbConfig::DbConfig_TmpDirPath).toString();
    return r;
}

bool DbConfig::setTmpDir(QString path)
{
    QSqlQuery q("SELECT COUNT(*) FROM config");
    int rows = 0;
    if (q.first())
        rows = q.value(0).toInt();
    if (rows == 0)
        q.prepare("INSERT INTO config (tmpdirpath) "
        "VALUES (:path)");
    else
        q.prepare("UPDATE CONFIG SET tmpdirpath=:path");
    q.bindValue(":path", path);
    return q.exec();
}

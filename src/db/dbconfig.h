#ifndef DBCONFIG_H
#define DBCONFIG_H

#include <QObject>

class DbConfig : public QObject
{
    Q_OBJECT
public:
    DbConfig(QObject *parent = 0);
    static QString getTmpDir();
    static bool setTmpDir(QString);
    enum {
        DbConfig_TmpDirPath = 0
    };
};

#endif // DBCONFIG_H

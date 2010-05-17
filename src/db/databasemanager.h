#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class QSqlError;
class QFile;
class QSqlTableModel;

class DatabaseManager : public QObject
{
public:
	DatabaseManager(QObject *parent = 0);
	~DatabaseManager();

    static bool openDB();
    static bool deleteDB();
    static bool dbExists();
    static QSqlError lastError();
    static bool createDB();
    static int insertPlatform(QString name, QString filename = "");
    static QString getPlatform(int id);
    QSqlTableModel* getPlatforms();

    enum {
    Platform_Id = 0,
    Platform_Name = 1,
    Platform_Filename = 2
    };

private:
    //static QSqlDatabase db;
	static const QString DB_FILENAME;
    static const QString DB_TABLE_NAME_PLATFORM;
    static QString getDbPath();

};

#endif

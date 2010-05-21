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


private:
    //static QSqlDatabase db;
	static const QString DB_FILENAME;
    static QString getDbPath();

};

#endif

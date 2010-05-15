#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class QSqlError;
class QFile;

class DatabaseManager : public QObject
{
public:
	DatabaseManager(QObject *parent = 0);
	~DatabaseManager();

        static bool openDB();
	bool deleteDB();
	bool dbExists() const;
	QSqlError lastError();
	bool createDB() const;
        static int insertPlatform(QString name, QString filename = "");
	QString getPlatform(int id) const;

private:
        static QSqlDatabase db;
	static const QString DB_FILENAME;
        static QString getDbPath();
};

#endif

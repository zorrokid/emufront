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

	bool openDB();
	bool deleteDB();
	bool dbExists() const;
	QSqlError lastError();
	bool createDB() const;
	int insertPlatform(QString name, QString filename = 0) const;
	QString getPlatform(int id) const;

private:
	QSqlDatabase db;
	static const QString DB_FILENAME;
	QString getDbPath() const;
};

#endif

#ifndef DBCREATOR_H
#define DBCREATOR_H

#include <QObject>

class DbCreator : public QObject
{
public:

    DbCreator(QObject *parent = 0);
    bool deleteDB();
    static bool tableExists(QString);
    static bool dbExists();
    bool createDB();

private:

    static const int TABLES_COUNT;
    static const QString TABLES[];

};

#endif // DBCREATOR_H

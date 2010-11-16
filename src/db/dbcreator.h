// EmuFront
// Copyright 2010 Mikko Keinänen
//
// This file is part of EmuFront.
//
//
// EmuFront is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation and appearing in the file gpl.txt included in the
// packaging of this file.
//
// EmuFront is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DBCREATOR_H
#define DBCREATOR_H

#include <QObject>

class DbCreator : public QObject
{
public:
    static const int DB_VERSION;

    DbCreator(QObject *parent = 0);
    bool deleteDB();
    static bool tableExists(QString);
    static int dbExists();
    bool createDB();

private:

    //static const int TABLES_COUNT;
    //static const QString TABLES[];

};

#endif // DBCREATOR_H

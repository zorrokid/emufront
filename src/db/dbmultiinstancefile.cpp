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

#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlRelationalTableModel>
#include "dbmultiinstancefile.h"

DbMultiInstanceFile::DbMultiInstanceFile(QObject *parent) :
    DbFile(parent)
{
}

int DbMultiInstanceFile::insertDataObjectToModel(const EmuFrontObject *efo)
{
    const EmuFrontFile *fi = dynamic_cast<const EmuFrontFile*>(efo);
    QSqlQuery q;
    q.prepare("INSERT INTO file "
              "(id, name, type, checksum, size, updatetime) "
              "VALUES (NULL, "", :type, :checksum, :size, :updatetime)"); // NOTE: in this class it is ok to leave the name field empty!
    q.bindValue(":type", fi->getType());
    q.bindValue(":checksum", fi->getCheckSum());
    q.bindValue(":size", fi->getSize());
    q.bindValue(":updatetime", DatabaseManager::getCurrentTimeStamp());
    int id = -1;
    if (q.exec())
        id = q.lastInsertId().toInt();
    return id;
}

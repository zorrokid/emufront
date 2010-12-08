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

#include "filepathmodel.h"
#include <QtSql>

FilePathModel::FilePathModel(QObject *parent) :
    EmuFrontQueryModel(parent)
{
    refresh();
}

void FilePathModel::refresh()
{

    // TODO
}

QString FilePathModel::constructSelect(QString where) const
{
    return QString("SELECT "
                   "filepath.id AS FilePathId, "
                   "filepath.name AS Name, "
                   "datetime(filepath.lastscanned, 'unixepoch') AS LastScanned, "
                   "setup.id AS SetupId, "
                   "platform.name || ' ' || mediatype.name AS SetupName, "
                   "filepath.filetypeid "
                   "FROM filepath "
                   "INNER JOIN setup ON filepath.setupid=setup.id  "
                   "INNER JOIN platform ON setup.platformid=platform.id "
                   "INNER JOIN mediatype ON setup.mediatypeid=mediatype.id "
                   "%1 "
                   "ORDER BY SetupName").arg(where);
}

Qt::ItemFlags FilePathModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    // TODO
    return flags;
}

bool FilePathModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // TODO
    return false;
}

bool FilePathModel::insertRows(int row, int count, const QModelIndex &parent)
{
    // TODO
    return false;
}

bool FilePathModel::removeRows(int row, int count, const QModelIndex &parent)
{
    // TODO
    return false;
}

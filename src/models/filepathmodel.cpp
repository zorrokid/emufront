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
    // TODO
    return QString();
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

/*
** EmuFront
** Copyright 2010 Mikko Keinänen
**
** This file is part of EmuFront.
**
**
** EmuFront is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License version 2 as published by
** the Free Software Foundation and appearing in the file gpl.txt included in the
** packaging of this file.
**
** EmuFront is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "externalexecutablemodel.h"

ExternalExecutableModel::ExternalExecutableModel(QObject *parent) :
    EmuFrontQueryModel(parent)
{
    editableColumns << Executable_Name;
    editableColumns << Executable_Options;
    editableColumns << Executable_Executable;
    editableColumns << Executable_SetupId;
    refresh();
}

void ExternalExecutableModel::refresh()
{
    setQuery(constructSelect());
    setHeaderData(Executable_Id, Qt::Horizontal, tr("Id"));
    setHeaderData(Executable_Name, Qt::Horizontal, tr("Name"));
    setHeaderData(Executable_Executable, Qt::Horizontal, tr("Executable"));
    setHeaderData(Executable_Options, Qt::Horizontal, tr("Options"));
    setHeaderData(Executable_TypeId, Qt::Horizontal, tr("Type"));
    setHeaderData(Executable_SetupId, Qt::Horizontal, tr("Setup id"));
    setHeaderData(Executable_SetupName, Qt::Horizontal, tr("Setup"));
}

QString ExternalExecutableModel::constructSelect(QString where) const
{
    return QString("SELECT "
                   "executable.id AS ExecutableId, "
                   "executable.name AS ExecutableName, "
                   "executable.executable AS Executable, "
                   "executable.options AS ExecutableOptions, "
                   "executable.type AS ExecutableType, "
                   "setup.id As ExecutableSetupId, "
                   "platform.name || ' ' || mediatype.name AS SetupName "
                   "FROM executable "
                   "INNER JOIN setup ON executable.setupid = setup.id "
                   "INNER JOIN platform ON setup.platformid=platform.id "
                   "INNER JOIN mediatype ON setup.mediatypeid=mediatype.id "
                   "%1 "
                   "ORDER BY executable.name").arg(where);
}

Qt::ItemFlags ExternalExecutableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    int col = index.column();
    if (editableColumns.contains(index.column())) {
       flags  |= Qt::ItemIsEditable;
    }
    return flags;
}

bool ExternalExecutableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!editableColumns.contains(index.column()))
        return false;

    QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(), Executable_Id);
    int id = data(primaryKeyIndex).toInt();
    clear();
    bool ok;
    return ok;
}

bool ExternalExecutableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    // TODO
    return false;
}

bool ExternalExecutableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    // TODO
    return false;
}

bool ExternalExecutableModel::setSetup(int isd, int setupId)
{
    // TODO
    return false;
}

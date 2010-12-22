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
#include "executable.h"
#include "setup.h"
#include "setupmodel.h"
#include "emufrontexception.h"
#include <QtSql>

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
                   " %1 "
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
    switch(index.column())
    {
        case Executable_Name:
            ok = setExecutableName(id, value.toString());
            break;
        case Executable_Executable:
            ok = setExecutable(id, value.toString());
            break;
        case Executable_Options:
            ok = setOptions(id, value.toString());
            break;
        case Executable_SetupId:
            ok = setSetup(id, value.toInt());
            break;
        default:
            ok = false;
    }
    refresh();
	if (ok) emit dataChanged();
    return ok;
}

bool ExternalExecutableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
        return false; // This is a flat model
    if (rowCount() < row)
        row = rowCount() + 1;

    // Fetch a setup for an initial selection
    int supId = -1;
    QSqlQuery q;
    q.exec(QString("SELECT setup.id, "
           // The following is to get the correct order:
           "platform.name || ' ' || mediatype.name AS SetupName "
           "FROM setup "
           "INNER JOIN platform ON setup.platformid=platform.id "
           "INNER JOIN mediatype ON setup.mediatypeid=mediatype.id "
           "ORDER BY SetupName "
           "LIMIT 1"));
    if (q.first()) {
        supId = q.value(0).toInt();
        qDebug() << "Got id " << supId << " for default setup.";
    }
    else {
        throw EmuFrontException(tr("No setups yet available for file path configuration!"));
    }
    q.prepare(QString("INSERT INTO executable "
        "(id, name, executable, options, type, setupid) "
        "VALUES (NULL, '', '', '', :type, :supid)"
    ));
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; ++i) {
        q.bindValue(":supid", supId);
        q.bindValue(":type", Executable::ExecutableType_Emulator);
        if (!q.exec()) {
            throw EmuFrontException(tr("Failed creating new external executable row: %1").
                                    arg(q.lastError().text()));
        }
    }
    endInsertRows();
    refresh();
    return true;
}

bool ExternalExecutableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid()) {
        return false; // This is a flat model
    }
    if (rowCount() < row + count - 1)
        return false;

    QSqlQuery q;
    q.prepare(QString("DELETE FROM executable WHERE id=:id"));
    QModelIndex primaryIndex;
    int id = -1;
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; ++i) {
        primaryIndex = QSqlQueryModel::index(row + i, Executable_Id);
        id = data(primaryIndex).toInt();
        qDebug() << "Removing data item with id " << id;
        q.bindValue(":id", id);
        q.exec();
    }
    endRemoveRows();
    refresh();
    return true;
}

bool ExternalExecutableModel::setSetup(int id, int setupId)
{
    QSqlQuery q;
    q.prepare(QString("UPDATE executable SET setupid = :supid WHERE id = :id"));
    q.bindValue(":supid", setupId);
    q.bindValue(":id", id);
    return q.exec();
}

bool ExternalExecutableModel::setExecutable(int id, QString name)
{
    QSqlQuery q;
    q.prepare(QString("UPDATE executable SET executable = :exec WHERE id = :id"));
    q.bindValue(":exec", name);
    q.bindValue(":id", id);
    return q.exec();
}

bool ExternalExecutableModel::setOptions(int id, QString options)
{
    QSqlQuery q;
    q.prepare(QString("UPDATE executable SET options = :opts WHERE id = :id"));
    q.bindValue(":opts", options);
    q.bindValue(":id", id);
    return q.exec();
}

bool ExternalExecutableModel::setExecutableName(int id, QString name)
{
    QSqlQuery q;
    q.prepare(QString("UPDATE executable SET name = :name WHERE id = :id"));
    q.bindValue(":name", name);
    q.bindValue(":id", id);
    return q.exec();
}

void ExternalExecutableModel::filterBySetup(int setupid)
{
    QList<QString> filters;
    filters.append(QString("executable.setupid=%1").arg(setupid));
    filterDataObjects(filters);
}

// Implemented for EmuFrontQueryModel:
EmuFrontObject* ExternalExecutableModel::recordToDataObject(const QSqlRecord* rec)
{
    Executable *ex = 0;
    if (!rec) return ex;
    int id = rec->value(Executable_Id).toInt();
    int supid = rec->value(Executable_SetupId).toInt();
    SetupModel supModel;
    EmuFrontObject *ob = supModel.getDataObject(supid);
    Setup *sup = dynamic_cast<Setup*>(ob);
    QString name = rec->value(Executable_Name).toString();
    QString exec = rec->value(Executable_Executable).toString();
    QString opts = rec->value(Executable_Options).toString();
    int type = rec->value(Executable_TypeId).toInt();
    ex = new Executable(id, name, exec, opts, sup, type);
    return ex;
}

QString ExternalExecutableModel::constructFilterById(int id) const
{
    return QString("executable.id=%1").arg(id);
}

Executable* ExternalExecutableModel::getExecutable(int row)
{
    if (row < 0) return 0;
    EmuFrontObject *efo = getObject(row);
    return dynamic_cast<Executable*>(efo);
}

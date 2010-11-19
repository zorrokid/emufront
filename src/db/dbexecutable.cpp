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
// but WITHOUT ANY WARRANTY{} without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#include <QDebug>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include "dbexecutable.h"
#include "dbsetup.h"
#include "../dataobjects/executable.h"


DbExecutable::DbExecutable(QObject *parent)
    : DbQueryModelManager(parent)
{
    dbSetup = new DbSetup(this);
    tableName = DbExecutable::DB_TABLE_EXECUTABLE;
}


/* Throws EmuFrontException */
EmuFrontObject* DbExecutable::recordToDataObject(const QSqlRecord* rec)
{
    Executable *ex = 0;
    if (!rec) return ex;
    int id = rec->value(Executable_Id).toInt();
    int supid = rec->value(Executable_SetupId).toInt();
    EmuFrontObject *ob = dbSetup->getDataObject(supid); /* Throws EmuFrontException */
    Setup *sup = dynamic_cast<Setup*>(ob);
    qDebug() << "Setup id " << sup->getId() << ", platform " << sup->getPlatform()->getName();
    QString name = rec->value(Executable_Name).toString();
    QString exec = rec->value(Executable_Executable).toString();
    QString opts = rec->value(Executable_Options).toString();
    int type = rec->value(Executable_TypeId).toInt();
    ex = new Executable(id, name, exec, opts, sup, type);
    return ex;
}

bool DbExecutable::updateDataObjectToModel(const EmuFrontObject* ob)
{
    const Executable *ex = dynamic_cast<const Executable*>(ob);
    bool ret = false;
    QSqlQuery q;
    q.prepare("UPDATE executable SET "
              "name=:name, "
              "executable=:executable, "
              "options=:options, "
              "setupid=:setupid, "
              "type=:type "
              "WHERE id=:id");
    // TODO: null check
    q.bindValue(":setupid", ex->getSetup()->getId());
    q.bindValue(":name", ex->getName());
    q.bindValue(":executable", ex->getExecutable());
    q.bindValue(":options", ex->getOptions());
    q.bindValue(":type", ex->getType());
    q.bindValue(":id", ex->getId());
    ret = q.exec();
    if (ret) resetModel();
    if (!ret)
        qDebug() << q.lastError().text();
    return ret;
}

int DbExecutable::insertDataObjectToModel(const EmuFrontObject* ob)
{
    const Executable *ex = dynamic_cast<const Executable*>(ob);
    QSqlQuery q;
    q.prepare("INSERT INTO executable "
        "(id, name, executable, options, setupid, type) "
        "VALUES (NULL, :name, :executable, :options, :setupid, :type)");

    if (!ex->getSetup() || ex->getSetup()->getId() < 0) {
        qDebug() << "Setup not available!";
        return -1;
    }
    q.bindValue(":setupid", ex->getSetup()->getId());
    q.bindValue(":name", ex->getName());
    q.bindValue(":executable", ex->getExecutable());
    q.bindValue(":options", ex->getOptions());
    q.bindValue(":type", ex->getType());
    int id = -1;
    if (q.exec())
        id = q.lastInsertId().toInt();
    else qDebug() << q.lastError().text();
    return id;
}

bool DbExecutable::deleteDataObjectFromModel(QModelIndex*)
{
    // TODO
    return false;
}

QString DbExecutable::constructSelectById(int id) const
{
    return constructSelect(
        QString("WHERE %1").arg(constructFilterById(id)));
}

QString DbExecutable::constructFilterById(int id) const
{
    return QString("executable.id=%1").arg(id);
}

QString DbExecutable::constructSelect(QString whereClause) const
{
    QString sql = QString("SELECT "
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
        "ORDER BY executable.name ").arg(whereClause);
    qDebug() << sql;
    return sql;
}

/*bool DbExecutable::deleteDataObject(int id) const
{
    // TODO
    return false;
}*/

QSqlQueryModel* DbExecutable::getData()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QString select = constructSelect();
    qDebug() << select;
    model->setHeaderData(Executable_Id, Qt::Horizontal, tr("Id"));
    model->setHeaderData(Executable_Name, Qt::Horizontal, tr("Name"));
    model->setHeaderData(Executable_Executable, Qt::Horizontal, tr("Executable"));
    model->setHeaderData(Executable_Options, Qt::Horizontal, tr("Options"));
    model->setHeaderData(Executable_TypeId, Qt::Horizontal, tr("Type"));
    model->setHeaderData(Executable_SetupId, Qt::Horizontal, tr("Setup id"));
    model->setHeaderData(Executable_SetupName, Qt::Horizontal, tr("Setup"));
    model->setQuery(select);
    return model;
}

QString DbExecutable::getCountRefsSelect(int id) const
{
    // These objects don't have references from other objects
    // currently.
    return QString("SELECT 0");
}

void DbExecutable::filterByPlatformMediaType(int platformId, int mediaTypeId)
{
    QList<QString> filters;
    filters.append(QString("setup.platformid=%1").arg(platformId));
    filters.append(QString("setup.mediatypeid=%1").arg(mediaTypeId));
    filterDataObjects(filters);
}

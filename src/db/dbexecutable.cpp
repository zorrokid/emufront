// EmuFront
// Copyright 2010 Mikko Keinänen
//
// This file is part of EmuFront.
//
//
// EmuFront is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
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
}

EmuFrontObject* DbExecutable::recordToDataObject(const QSqlRecord* rec)
{
    Executable *ex = 0;
    if (!rec) return ex;
    int id = rec->value(Executable_Id).toInt();
    int supid = rec->value(Executable_SetupId).toInt();
    Setup *sup = dynamic_cast<Setup*>(dbSetup->getDataObject(supid));
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
    q.bindValue(":setupid", ex->getSetup()
                ? QString(ex->getSetup()->getId()) : "NULL"); // TODO: null shouln't be allowed here
    q.bindValue(":name", ex->getName());
    q.bindValue(":executable", ex->getExecutable());
    q.bindValue(":options", ex->getOptions());
    q.bindValue(":type", ex->getType());
    q.bindValue(":id", ex->getId());
    ret = q.exec();
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

int DbExecutable::countDataObjectRefs(int) const
{
    // TODO
    return 0;
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
    return QString("SELECT "
        "executable.id AS ExecutableId, "
        "executable.name AS ExecutableName, "
        "executable.executable AS Executable, "
        "executable.options AS ExecutableOptions, "
        "executable.type AS ExecutableType, "
        "setup.id As ExecutableSetupId "
        "platform.name || ' ' || mediatype.name AS SetupName "
        "FROM executable "
        "INNER JOIN setup ON executable.setupid = setup.id "
        "INNER JOIN platform ON setup.platformid=platform.id "
        "INNER JOIN mediatype ON setup.mediatypeid=mediatype.id "
        "%1 "
        "ORDER BY executable.name ")
        .arg(whereClause);
}

bool DbExecutable::deleteDataObject(int id) const
{
    // TODO
    return false;
}

QSqlQueryModel* DbExecutable::getData()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString select = constructSelect();
    // TODO ...
    model->setQuery(select);
    return model;
}

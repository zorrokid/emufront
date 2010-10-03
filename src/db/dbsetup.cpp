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
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#include <QDebug>
#include <QStringList>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include "dbsetup.h"

const QString DbSetup::FILE_TYPE_EXTENSION_SEPARATOR = QString("|");

DbSetup::DbSetup(QObject *parent) : DbQueryModelManager(parent)
{
    dbPlatform = new DbPlatform(this);
    dbMediaType = new DbMediaType(this);
}

EmuFrontObject* DbSetup::recordToDataObject(const QSqlRecord *rec)
{
    Setup *s = 0;
    if (!rec) return s;
    int id = rec->value(Setup_Id).toInt();
    QString extensions = rec->value(Setup_FileTypeExtensions).toString();
    QStringList list = extensions.split(FILE_TYPE_EXTENSION_SEPARATOR);
    int plfId = rec->value(Setup_PlatformId).toInt();
    int mtId = rec->value(Setup_MediaTypeId).toInt();
    Platform *plf = dynamic_cast<Platform*>(dbPlatform->getDataObject(plfId));
    MediaType *mt = dynamic_cast<MediaType*>(dbMediaType->getDataObject(mtId));
    s = new Setup(id, plf, mt, list);
    return s;
}

bool DbSetup::updateDataObjectToModel(const EmuFrontObject *ob)
{
    const Setup *fpo = dynamic_cast<const Setup*>(ob);
    bool ret = false;
    QSqlQuery query;
    query.prepare(QString("UPDATE setup SET "
        "platformid=:platformid, "
        "mediatypeid=:mediatypeid, "
        "filetypeextensions=:filetypeextensions "
        "WHERE id = :id"));
    if (fpo->getPlatform())
        query.bindValue(":platformid", fpo->getPlatform()->getId());
    if (fpo->getMediaType())
        query.bindValue(":mediatypeid", fpo->getMediaType()->getId());
    query.bindValue(":filetypeextensions", fpo->getSupportedFileTypeExtensions().join(FILE_TYPE_EXTENSION_SEPARATOR));
    query.bindValue(":id", fpo->getId());
    ret = query.exec();
    if (!ret) qDebug() << query.lastError().text() << query.executedQuery();
    return ret;
}

int DbSetup::insertDataObjectToModel(const EmuFrontObject *ob)
{
    qDebug() << "Inserting setup to database...";
    const Setup *fpo = dynamic_cast<const Setup*>(ob);
    QSqlQuery query;
    query.prepare("INSERT INTO setup (id, platformid, mediatypeid, filetypeextensions)"
        "VALUES (NULL, :platformid, :mediatypeid, :fileextensions)");
    int plfId = fpo->getPlatform() ? fpo->getPlatform()->getId() : -1;
    int mtId = fpo->getMediaType() ? fpo->getMediaType()->getId() : -1;
    QString exts = fpo->getSupportedFileTypeExtensions().join(FILE_TYPE_EXTENSION_SEPARATOR);
    qDebug() << "Going to insert setup with platform " << plfId << ", media type " << mtId << " and extensions " << exts;
    query.bindValue(":platformid", plfId);
    query.bindValue(":mediatypeid", mtId);
    query.bindValue(":filetypeextensions", exts);
    int id = -1;
    if (query.exec())
        id = query.lastInsertId().toInt();
    else
        qDebug() << query.lastError().text() << query.executedQuery();
    return id;
}

int DbSetup::countDataObjectRefs(int ) const
{
    return 0;
}

QString DbSetup::constructSelect(QString where) const
{
    /*QString where = whereClause.isEmpty()
        ? "" : QString("WHERE ").append(whereClause);*/
    return QString(
        "SELECT setup.id AS SetupId, "
        "setup.platformid AS PlatformId, "
        "setup.mediatypeid AS MediaTypeId, "
        "setup.filetypeextensions AS SupportedFileTypeExtensions, "
        "platform.name || ' ' || mediatype.name AS SetupName "
        "FROM setup "
        "INNER JOIN platform ON setup.platformid=platform.id "
        "INNER JOIN mediatype ON setup.mediatypeid=mediatype.id %1 "
        "ORDER BY SetupName"
        ).arg(where);
}

QString DbSetup::constructFilterById(int id) const
{
     return QString("setup.id = %1").arg(id);
}


QString DbSetup::constructSelectById(int id) const
{
    return constructSelect(QString("WHERE %1").append(constructFilterById(id)));
}

// WARNING: this will delete also all the databindings to selected media image path
bool DbSetup::deleteDataObjectFromModel(QModelIndex */*index*/)
{
    qDebug() << "This is not currently supported";
    return false;
}

bool DbSetup::deleteDataObject(int id) const
{
    if (countDataObjectRefs(id) > 0)
        // TODO
        return false;
    QSqlQuery q;
    q.prepare(QString("DELETE FROM setup WHERE id=:id"));
    q.bindValue(":id", id);
    return q.exec();
}

QSqlQueryModel* DbSetup::getData()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString select = constructSelect();
    qDebug() << select;
    model->setQuery(select);
    model->setHeaderData(Setup_Id, Qt::Horizontal, tr("Id"));
    model->setHeaderData(Setup_PlatformId, Qt::Horizontal, tr("Platform id"));
    model->setHeaderData(Setup_MediaTypeId, Qt::Horizontal, tr("Media type id"));
    model->setHeaderData(Setup_FileTypeExtensions, Qt::Horizontal, tr("File types"));
    model->setHeaderData(Setup_Name, Qt::Horizontal, tr("Name"));
    return model;
}

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
    tableName = DbSetup::DB_TABLE_NAME_SETUP;
    dbPlatform = new DbPlatform(this);
    dbMediaType = new DbMediaType(this);
}

EmuFrontObject* DbSetup::recordToDataObject(const QSqlRecord *rec)
{
    Setup *s = 0;
    if (!rec) return s;
    int id = rec->value(Setup_Id).toInt();
    QString extensions = rec->value(Setup_FileTypeExtensions).toString().trimmed();
    QStringList list;
    if (!extensions.isEmpty())
        list = extensions.split(FILE_TYPE_EXTENSION_SEPARATOR);
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
    query.bindValue(":filetypeextensions", supportedExtensionsToDb(fpo->getSupportedFileTypeExtensions()));
    query.bindValue(":id", fpo->getId());
    ret = query.exec();
    if (!ret) qDebug() << query.lastError().text() << query.executedQuery();
    return ret;
}

QString DbSetup::supportedExtensionsToDb(QStringList list)
{
    return list.isEmpty() ? "" : list.join(FILE_TYPE_EXTENSION_SEPARATOR);
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
    query.bindValue(":platformid", plfId);
    query.bindValue(":mediatypeid", mtId);
    query.bindValue(":filetypeextensions", supportedExtensionsToDb(fpo->getSupportedFileTypeExtensions()));
    int id = -1;
    if (query.exec())
        id = query.lastInsertId().toInt();
    else
        qDebug() << query.lastError().text() << query.executedQuery();
    return id;
}

QString DbSetup::constructSelect(QString where) const
{
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
    return constructSelect(
        QString("WHERE %1").arg(constructFilterById(id)));
}

// WARNING: this will delete also all the databindings to selected media image path
bool DbSetup::deleteDataObjectFromModel(QModelIndex */*index*/)
{
    qDebug() << "This is not currently supported";
    return false;
}

QSqlQueryModel* DbSetup::getData()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
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

QString DbSetup::getCountRefsSelect(int id) const
{
    /* setups are referenced by executable and filepath */
    return QString("SELECT count(*) FROM "
            "(SELECT setup.id FROM setup "
              "INNER JOIN executable ON setup.id=executable.setupid "
              "WHERE setup.id=%1 "
              "UNION ALL "
              "SELECT setup.id FROM setup "
              "INNER JOIN filepath ON setup.id=filepath.setupid "
              "WHERE setup.id=%1)").arg(id);
}

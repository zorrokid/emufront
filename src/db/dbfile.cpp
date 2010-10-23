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
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlRelationalTableModel>
#include "dbfile.h"


DbFile::DbFile(QObject *parent) : DbQueryModelManager(parent)
{
    tableName = DbFile::DB_TABLE_NAME_FILE;
    type = -1;
}

EmuFrontObject* DbFile::recordToDataObject(const QSqlRecord *record)
{
    int id = record->value(File_Id).toInt();
    QString name = record->value(File_Name).toString();
    QString checksum = record->value(File_CheckSum).toString();
    int size = record->value(File_FileSize).toInt();
    int type = record->value(File_FileType).toInt();
    return new EmuFrontFile(id, name, checksum, size, type);
}

bool DbFile::updateDataObjectToModel(const EmuFrontObject *ob)
{
    const EmuFrontFile *plf = dynamic_cast<const EmuFrontFile*>(ob);
    bool ret = false;

    QSqlQuery query;
    query.prepare(QString("UPDATE file SET"
                          "name=:name, "
                          "type=:type, "
                          "checksum=:checksum, "
                          "size=:size, "
                          "updatetime:=updatetime "
                          "WHERE id=:id"));
    query.bindValue(":name", plf->getName());
    query.bindValue(":type", plf->getType());
    query.bindValue(":checksum", plf->getCheckSum());
    query.bindValue(":size", plf->getSize());
    query.bindValue(":updatetime", getCurrentTimeStamp());
    ret = query.exec();
    if (ret) resetModel();
    return ret;
}

int DbFile::insertDataObjectToModel(const EmuFrontObject *ob)
{
    const EmuFrontFile *fi = dynamic_cast<const EmuFrontFile*>(ob);
    QSqlQuery q;
    q.prepare("INSERT INTO file "
              "(id, name, type, checksum, size, updatetime) "
              "VALUES (NULL, :name, :type, :checksum, :size, :updatetime)");
    q.bindValue(":name", fi->getName());
    q.bindValue(":type", fi->getType());
    q.bindValue(":checksum", fi->getCheckSum());
    q.bindValue(":size", fi->getSize());
    q.bindValue(":updatetime", DatabaseManager::getCurrentTimeStamp());
    int id = -1;
    if (q.exec())
        id = q.lastInsertId().toInt();
    return id;
}

// WARNING: this will delete also all the databindings to selected platform
// the delete must be confirmed in the UI
bool DbFile::deleteDataObjectFromModel(QModelIndex *index)
{
    return false;
}

bool DbFile::deleteDataObject(int id) const
{
    if (countDataObjectRefs(id) > 0)
        // TODO
        return false;
    QSqlQuery q;
    q.prepare(QString("DELETE FROM file WHERE id=:id"));
    q.bindValue(":id", id);
    return q.exec();
}

QString DbFile::constructSelect(QString where) const
{
    return QString("SELECT file.id AS FileId, "
                   "file.name AS Name, "
                   "file.type AS FileType, "
                   "file.checksum AS Checksum, "
                   "file.size AS FileSize, "
                   "file.updatetime AS UpdateTime "
                   "FROM file "
                   "%1 "
                   "ORDER BY Name").arg(where);
}

QString DbFile::constructFilterById(int id) const
{
    return QString("file.id = %1").arg(id);
}

QString DbFile::constructSelectById(int id) const
{
    return constructSelect(QString("WHERE %1").arg(constructFilterById(id)));
}

QSqlQueryModel* DbFile::getData()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(constructSelect());
    model->setHeaderData(File_Name, Qt::Horizontal, tr("Name"));
    model->setHeaderData(File_FileType, Qt::Horizontal, tr("Type"));
    model->setHeaderData(File_CheckSum, Qt::Horizontal, tr("Checksum"));
    model->setHeaderData(File_FileSize, Qt::Horizontal, tr("Size"));
    model->setHeaderData(File_UpdateTime, Qt::Horizontal, tr("Updated"));
    return model;
}

EmuFrontObject* DbFile::getFileByChecksum(QString checksum)
{
    return getDataObject(QString("checksum LIKE '%1'").arg(checksum));
}

QString DbFile::getCountRefsSelect(int id) const
{
    /* files are referenced from platform, mediatype, mediaimagecontainer_mediaimage and mediaimagecontainer. */
    return QString("SELECT count(*) FROM ("
              "SELECT file.id FROM file "
              "INNER JOIN platform ON file.id=platform.fileid "
              "WHERE file.id=%1 "
              "UNION ALL "
              "SELECT file.id FROM file "
              "INNER JOIN mediatype ON file.id=mediatype.fileid "
              "WHERE file.id=%1 "
              "UNION ALL "
              "SELECT file.id FROM file "
              "INNER JOIN mediaimagecontainer_mediaimage "
              "ON (mediaimagecontainerid=file.id OR mediaimageid=file.id) "
              "WHERE file.id=%1 "
              ")").arg(id);
}

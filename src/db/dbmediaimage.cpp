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
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include "dbmediaimage.h"


DbMediaImage::DbMediaImage(QObject *parent)
    : DbFile(parent)
{
    type = EmuFrontFile::FileType_MediaImage;
    tableName = DbMediaImage::DB_TABLE_NAME_FILE;
}

/*bool DbMediaImage::updateDataObjectToModel(const EmuFrontObject *efo)
{
    // TODO
    return false;
}

bool DbMediaImage::insertDataObjectToModel(const EmuFrontObject *efo)
{
    // TODO
    return false;
}

bool DbMediaImage::deleteDataObjectFromModel(QModelIndex *i)
{
    // TODO
    return false;
}

int DbMediaImage::countDataObjectRefs(int id) const
{
    // TODO
    return -1;
}

QString DbMediaImage::constructSelect(QString whereClause) const
{
    // TODO
    return "";
}

QString DbMediaImage::constructSelectById(int id) const
{
    // TODO
    return "";
}

EmuFrontObject* DbMediaImage::recordToDataObject(const QSqlRecord *)
{
    // TODO
    return 0;
}*/

/*QSqlQueryModel* DbMediaImage::getData()
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable(DB_TABLE_NAME_FILE);
    return model;
}*/

/*int DbMediaImage::insertMediaImage(const MediaImage *mi)
{
    return DbFile::insertDataObjectToModel(mi);
}*/

/* Stores a list of media images to the database.
   Returns a list of media image id corresponding to the given list of media
   images inserted to the database or already in the database.
*/
QList<int> DbMediaImage::storeMediaImages(QMap<QString, EmuFrontObject*> images)
{
    qDebug() << "Storing media images to database.";
    QList<int> ids  = QList<int>();
    QMapIterator<QString, EmuFrontObject*> it(images);
    MediaImage *mi = 0;
    while(it.hasNext())
    {
        it.next();
        mi = dynamic_cast<MediaImage*>(it.value());
        QString cksum = mi->getCheckSum();
        qDebug() << "Storing media image " << mi->getName()
            << " with checksum " << cksum;
        EmuFrontObject *o = getFileByChecksum(cksum);
        int id = o ? o->getId() : -1;
        if (id >= 0)
        {
            qDebug() << "This media image already exists with id " << id;
            delete o;
            // this media image is already in the database
            // TODO: what if the name differs? (cannot update to database, since the same media image
            // might be inside another container...
            // possible solution:
            // * store media image names in different
            //   table linked to the media image container
        }
        else if (id < 0)
        {
            qDebug() << "This media image is not yet in the db.";
            id = insertDataObjectToModel(mi);
            if (id < 0)
            {
                // TODO: Build an error message of failed inserts
                qDebug() << "Failed inserting media image" << mi->getName();
            }
        }
        if (id > 0) {
            ids.append(id);
            mi->setId(id);
        }
    }
    return ids;
}

void DbMediaImage::removeOrphanedMediaImages(QList<int> ids)
{
    // TODO
    // go through the list of media image ids,
    // if the media image with curr id doesn't have a container, delete it
}

/* Fetches a list of media images inside a media image container
    with a given id */
QMap<QString, EmuFrontObject*> DbMediaImage::getMediaImages(int micId) const
{
    QMap<QString, EmuFrontObject*> list;
    QSqlQuery  q;
    q.prepare("SELECT file.id, file.name, file.size, file.checksum "
        "FROM file INNER JOIN mediaimagecontainer_mediaimage "
        "ON mediaimagecontainer_mediaimage.mediaimageid = file.id "
        "WHERE mediaimagecontainer_mediaimage.mediaimagecontainerid = :micid ");
    q.bindValue(":micid", micId);
    q.exec();
    QSqlRecord rec;
    int id, size;
    QString name, checksum;
    MediaImage *mi = 0;
    while(q.next()) {
        // TODO: checks?
        rec = q.record();
        id = rec.value(DbMediaImage::File_Id).toInt();
        name = rec.value(DbMediaImage::File_Name).toString();
        checksum = rec.value(DbMediaImage::File_CheckSum).toString();
        size = rec.value(DbMediaImage::File_FileSize).toInt();
        list[checksum] = new MediaImage(id, name, checksum, size);
    }
    return list;
}

QString DbMediaImage::getCountRefsSelect(int id) const
{

    /* nothing will be removed if a mediaimage file is removed
       from the db. TODO: if all the mediaimages from
           mediaimagecontainer are removed
           the mediaimagecontainer should be removed! */
    return QString("SELECT 0");
    /*return QString("SELECT count(*) FROM file "
              "INNER JOIN mediaimagecontainer_mediaimage "
              "ON file.id=mediaimagecontainer_mediaimage.mediaimageid "
              "WHERE file.id=%1").arg(id);*/
}

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


/* Stores a list of media images to the database.
   Returns a list of media image id corresponding to the given list of media
   images inserted to the database or already in the database.
*/
QList<int> DbMediaImage::storeMediaImages(QMap<QString, EmuFrontObject*> images)
{
    QList<int> ids  = QList<int>();
    QMapIterator<QString, EmuFrontObject*> it(images);
    MediaImage *mi = 0;
    while(it.hasNext())
    {
        it.next();
        mi = dynamic_cast<MediaImage*>(it.value());
        int id = insertDataObjectToModel(mi);
        if (id < 0) {
            // TODO: Build an error message of failed inserts
            qDebug() << "Failed inserting media image" << mi->getName();
        }
        else if (id >= 0) {
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

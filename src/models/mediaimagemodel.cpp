/*
** EmuFront
** Copyright 2010-2011 Mikko Keinänen
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

#include "mediaimage.h"
#include "mediaimagemodel.h"
#include <QtSql>

MediaImageModel::MediaImageModel(QObject *parent) :
    FileModel(parent)
{
}

/* Fetches a list of media images inside a media image container
    with a given id */
QMap<QString, EmuFrontObject*> MediaImageModel::getMediaImages(int micId)
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
        id = rec.value(MediaImageModel::File_Id).toInt();
        name = rec.value(MediaImageModel::File_Name).toString();
        checksum = rec.value(MediaImageModel::File_CheckSum).toString();
        size = rec.value(MediaImageModel::File_FileSize).toInt();
        list[checksum] = new MediaImage(id, name, checksum, size);
    }
    return list;
}

/* Stores a list of media images to the database.
   Returns a list of media image id corresponding to the given list of media
   images inserted to the database or already in the database.
*/
QList<int> MediaImageModel::storeMediaImages(QMap<QString, EmuFrontObject*> images)
{
    QList<int> ids  = QList<int>();
    QMapIterator<QString, EmuFrontObject*> it(images);
    MediaImage *mi = 0;
    while(it.hasNext())
    {
        it.next();
        mi = dynamic_cast<MediaImage*>(it.value());
        int id = insertDataObject(mi);
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

void MediaImageModel::removeOrphanedMediaImages(QList<int> ids)
{
    // TODO
    // go through the list of media image ids,
    // if the media image with curr id doesn't have a container, delete it
}



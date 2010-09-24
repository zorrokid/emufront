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
#include <QSqlTableModel>
#include <QSqlQuery>
#include "dbmediaimage.h"

DbMediaImage::DbMediaImage(QObject *parent)
    : DbFile(parent)
{
    type = EmuFrontFile::FileType_MediaImage;
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
QList<int> DbMediaImage::storeMediaImages(QList<MediaImage *> images)
{
    qDebug() << "Storing media images to database.";
    QList<int> ids  = QList<int>();
    foreach(MediaImage* mi, images)
    {
        QString cksum = mi->getCheckSum();
        qDebug() << "Storing media image " << mi->getName()
            << " with checksum " << cksum;
        EmuFrontObject *o = getFileByChecksum(cksum);
        int id = o ? o->getId() : -1;
        if (id >= 0)
        {
            qDebug() << "This media image already exists with id " << id;
            // this media image is already in the database
            // TODO: what if the name differs? (cannot update to database, since the same media image
            // might be inside another container
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
        if (id > 0)
            ids.append(id);
    }
    return ids;
}

void DbMediaImage::removeOrphanedMediaImages(QList<int> ids)
{
    // TODO
    // go through the list of media image ids,
    // if the media image with curr id doesn't have a container, delete it
}

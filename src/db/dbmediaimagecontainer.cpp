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
#include <QSqlQuery>
#include <QSqlError>
#include "dbmediaimagecontainer.h"

DbMediaImageContainer::DbMediaImageContainer(QObject *parent)
    : DbFile(parent) // DbQueryModelManager(parent)
{
    dbMediaImage = new DbMediaImage(parent);
    //dbFile = new DbFile(parent);
}

bool DbMediaImageContainer::updateDataObjectToModel(const EmuFrontObject *efo)
{
    // TODO
    return false;
}

int DbMediaImageContainer::insertDataObjectToModel(const EmuFrontObject *efo)
{
    /* "CREATE TABLE IF NOT EXISTS mediaimagecontainer "
                        "(id INTEGER PRIMARY KEY, "
                        "fileid INTEGER REFERENCES file(id), "
                        "filepathid INTEGER REFERENCES filepath(id), "
                        "updatetime NUMERIC)"*/

    const MediaImageContainer *mic
        = dynamic_cast<const MediaImageContainer *>(efo);

    // Insert MediaImageContainer first as a EmuFrontFile object to file table.
    // File id is used to store the media image container instance to database.

    int fileId = DbFile::insertDataObjectToModel(mic);

    if (fileId < 0) {
        throw new EmuFrontException(
                QString(tr("Inserting media image container %1 to file database failed"))
                .arg(mic->getName()));

    }

    // Insert to mediaimagecontainer table

    QSqlQuery q;
    q.prepare("INSERT INTO mediaimagecontainer "
        "(id, fileid, filepathid, updatetime) "
        "VALUES (NULL, :fileid, :filepathid, :updatetime");

    return -1;
}

bool DbMediaImageContainer::deleteDataObjectFromModel(QModelIndex *i)
{
    // TODO
    return false;
}

int DbMediaImageContainer::countDataObjectRefs(int id) const
{
    // TODO
    return -1;
}

QString DbMediaImageContainer::constructSelect(QString whereClause) const
{
    // TODO
    return "";
}

QString DbMediaImageContainer::constructFilterById(int id) const
{
    // TODO
    return "";
}

QString DbMediaImageContainer::constructSelectById(int id) const
{
    // TODO
    return "";
}

EmuFrontObject* DbMediaImageContainer::recordToDataObject(const QSqlRecord *)
{
    // TODO
    return 0;
}

QSqlQueryModel* DbMediaImageContainer::getData()
{
    // TODO
    return 0;
}

int DbMediaImageContainer::getMediaImageContainer(QString checksum) const
{
    // TODO
    return -1;
}


/**
* Stores media image containers, including the media images included
* to database.
*/
void DbMediaImageContainer::storeContainers(QList<MediaImageContainer *> lst, FilePathObject *fpo)
{
    qDebug() << "Storing media image containers to database.";
    foreach(MediaImageContainer *mic, lst)
    {
        qDebug() << "Media image container " << mic->getName();
        QList<MediaImage*> images = mic->getMediaImages();

        /* If media image container is already in the db, continue */
        if (getMediaImageContainer(mic->getCheckSum()) >= 0)
            continue;

        // this is a new media image container, lets build a list
        // of media image id's for this container
        QList<int> ids = dbMediaImage->storeMediaImages(images);

        if (ids.count() > 0)
        {
            try {
                // mediaimagecontainer table: id, fileid, filepathid, updatetime

                // insert the media image container file to file table
                int micFileId = DbFile::insertDataObjectToModel(mic);
                if (micFileId < 0) {
                    throw new EmuFrontException(
                        QString(tr("Inserting media image container %1 to file database failed"))
                            .arg(mic->getName()));
                }
                int fpId = fpo->getId();
                // store media image to db
                int micId = insertDataObjectToModel(mic);
                if (micId < 0){
                    // because the previous insert failed, the next is most likely going to fail, throw exception
                    throw new EmuFrontException(
                        QString(tr("Failed inserting media image container '%1' to database!"))
                            .arg(mic->getName()));
                }

                // link all the media image ids in list to media image container id
                linkMediaImagesWithContainer(micId, ids);
            } catch (EmuFrontException e) {
                // need to remove the media images without media image container in list 'ids'
                dbMediaImage->removeOrphanedMediaImages(ids);
            }
        }
    }
}

void DbMediaImageContainer::linkMediaImagesWithContainer(int micId, QList<int> miIds)
{
    // TODO
}

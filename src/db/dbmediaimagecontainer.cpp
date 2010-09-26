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
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlRelationalTableModel>
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

    if (!mic->getFilePath())
        // TODO: note we most surely need to catch the exception
        // in the calling code block and clean
        // all the media image and ...containers from
        // the memory!
        throw new EmuFrontException("Cannot install media image "
            "container to database without a file path object!");

    // Insert MediaImageContainer first as a EmuFrontFile object to file table.
    // File id is used to store the media image container instance to database,
    // file id is also the media image container id
    int fileId = DbFile::insertDataObjectToModel(mic);

    if (fileId < 0) {
        // TODO: note we most surely need to catch the exception
        // in the calling code block and clean
        // all the media image and ...containers from
        // the memory!
        throw new EmuFrontException(
                QString(tr("Inserting media image container %1 to file database failed"))
                .arg(mic->getName()));
    }

    // Insert to mediaimagecontainer table

    QSqlQuery q;
    q.prepare("INSERT INTO mediaimagecontainer "
        "(fileid, filepathid, updatetime) "
        "VALUES (:fileid, :filepathid, :updatetime)");
    q.bindValue(":fileid", fileId);
    q.bindValue(":filepathid", mic->getFilePath()->getId());
    q.bindValue(":updatetime", DatabaseManager::getCurrentTimeStamp());
    if (!q.exec())
        // TODO: failed inserting, remove orphaned media images
        // (this is actually done in the storeContainers catch block
        // but maybe it should be done here also, if this function is called independently!
        // TODO: note we most surely need to catch the exception
        // in the calling code block and clean
        // all the media image and ...containers from
        // the memory!
        throw new EmuFrontException("Failed inserting media image to database!");
    return fileId;
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
                int micFileId = insertDataObjectToModel(mic);
                if (micFileId < 0) {
                    // TODO: note we most surely need to catch the exception
                    // in the calling code block and clean
                    // all the media image and ...containers from
                    // the memory!
                    throw new EmuFrontException(
                        QString(tr("Inserting media image container %1 to file database failed"))
                            .arg(mic->getName()));
                }
                // link all the media image ids in list to media image container id
                linkMediaImagesWithContainer(micFileId, ids);
            } catch (EmuFrontException e) {
                // need to remove the media images without media image container in list 'ids'
                    // TODO: clean
                    // all the media image and ...containers from
                    // the memory! (maybe throw another exception on calling code block for this)

                dbMediaImage->removeOrphanedMediaImages(ids);
            }
        }
    }
}

void DbMediaImageContainer::linkMediaImagesWithContainer(int micId, QList<int> miIds)
{
    // TODO
}

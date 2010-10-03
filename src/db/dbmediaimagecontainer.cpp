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
    const MediaImageContainer *mic
        = dynamic_cast<const MediaImageContainer *>(efo);

    // check if this media image container is already in the database
    EmuFrontObject *o = getFileByChecksum(mic->getCheckSum());
    int fileId = o ? o->getId() : -1;
    /*int fileId = getMediaImageContainer(mic->getCheckSum());*/
    if (fileId >= 0) {
        qDebug() << "Media image container already in db with id " << fileId << ".";
        return fileId;
   }

    if (!mic->getFilePath())
        throw new EmuFrontException("Cannot install media image "
            "container to database without a file path object!");

    QList<MediaImage*> images = mic->getMediaImages();
    QList<int> ids = dbMediaImage->storeMediaImages(images);

    qDebug() << "Stored " << ids.count() << " media images.";

    if (ids.count() <= 0)
        return -1;

    /* Contained Media images successfully stored to db,
        storing media image container also */

    try {

        // Insert MediaImageContainer first as a EmuFrontFile object to file table.

        // File id is used to store the media image container instance to database,
        // file id is also the media image container id
        fileId = DbFile::insertDataObjectToModel(mic);

        qDebug() << "Inserted media image container to file table with id " << fileId << ".";

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
        if (!q.exec()){
            DbFile::deleteDataObject(fileId);
            throw new EmuFrontException("Failed inserting media image to database!");
        }
        qDebug() << "Inserted media image container " << fileId << " to mediaimagecontainer table.";
        linkMediaImagesWithContainer(fileId, ids);
        qDebug() << "Linked media image container with media images.";
    } catch (EmuFrontException e) {
        dbMediaImage->removeOrphanedMediaImages(ids);
        throw e;
    }

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
    // TODO, for a usual search we need a "light" version of this select
    // and MediaImageContainer (only id, name)
    return QString("SELECT file.id, file.name, file.checksum, file.size, "
                "        filepath.id, filepath.name, "
                "        setup.id, "
                "        platform.id, platform.name, "
                "        mediatype.id, mediatype.name "
                "FROM mediaimagecontainer "
                "INNER JOIN file ON mediaimagecontainer.fileid = file.id "
                "INNER JOIN filepath ON mediaimagecontainer.filepathid = filepath.id "
                "INNER JOIN setup ON filepath.setupid = setup.id "
                "INNER JOIN platform ON setup.platformid = platform.id "
                "INNER JOIN mediatype ON setup.mediatypeid = mediatype.id "
                "%1 "
                "ORDER BY file.name").arg(whereClause);
}

QString DbMediaImageContainer::constructFilterById(int id) const
{
    return DbFile::constructFilterById(id);
}

QString DbMediaImageContainer::constructSelectById(int id) const
{
    return DbFile::constructSelectById(id);
}

EmuFrontObject* DbMediaImageContainer::recordToDataObject(const QSqlRecord *rec)
{
    return DbFile::recordToDataObject(rec);
}

QSqlQueryModel* DbMediaImageContainer::getData()
{
    return DbFile::getData();
}

/* Returns the id of a media image container with a given cheksum or -1 if not found */
int DbMediaImageContainer::getMediaImageContainer(QString checksum) const
{
    QSqlQuery q;
    q.prepare("SELECT id FROM file WHERE checksum=:checksum");
    q.bindValue(":checksum", checksum);
    int id = -1;
    if (q.next())
        id = q.value(0).toInt();
    return id;
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
        int micFileId = insertDataObjectToModel(mic);
    }
}

void DbMediaImageContainer::linkMediaImagesWithContainer(int micId, QList<int> miIds)
{
    if (micId < 0 || miIds.count() <= 0)
        return;

    QSqlQuery q;
    q.prepare("INSERT INTO mediaimagecontainer_mediaimage "
        "(mediaimagecontainerid, mediaimageid) "
        "VALUES (:micid, :miid) ");
    q.bindValue(":micid", micId);

    foreach(int miid, miIds) {
        qDebug() << "Linking media image container " << micId << " to media image " << miid  << ".";
        q.bindValue(":miid", miid);
        if (!q.exec()) {
            throw new EmuFrontException(QString("Failed linking media "
                "image container %1 to a media image %2").arg(micId).arg(miid));
        }
    }
}

void DbMediaImageContainer::filter(int mediaTypeId, int platformId)
{
    QList<QString> filters;
    if (mediaTypeId >= 0)
        filters.append(QString("mediatype.id=%1").arg(mediaTypeId));
    if (platformId >= 0)
        filters.append(QString("platform.id=%1").arg(platformId));
    filterDataObjects(filters);
}


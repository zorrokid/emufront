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
#include <QSqlError>
#include "dbmediaimagecontainer.h"
#include "dbmediaimage.h"
//#include "dbsetup.h"
#include "dbfilepath.h"

DbMediaImageContainer::DbMediaImageContainer(QObject *parent)
    : DbFile(parent)
{
    dbMediaImage = new DbMediaImage(parent);
    dbFilePath = new DbFilePath(parent);
    tableName = DbMediaImageContainer::DB_TABLE_MEDIAIMAGECONTAINER;
}

bool DbMediaImageContainer::updateDataObjectToModel(const EmuFrontObject *efo)
{
    // TODO
    return false;
}


int DbMediaImageContainer::storeMediaImageContainer(EmuFrontObject *efo)
{
    MediaImageContainer *mic
        = dynamic_cast<MediaImageContainer *>(efo);

    if (!mic->getFilePath())
        throw new EmuFrontException("Cannot install media image "
            "container to database without a file path object!");

    // multiple media image containers with matching checksum will be stored
    //       if each instance is in a different file path

    int fileId = -1;
    QMap<QString, EmuFrontObject*> images = mic->getMediaImages();
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

        fileId = insertDataObjectToModel(mic);

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

        mic->setId(fileId);

        if (!linkMediaImageContainerToPath(mic)){
            DbFile::deleteDataObject(fileId);
            throw new EmuFrontException("Failed inserting media image to database!");
        }
        qDebug() << "Inserted media image container " << fileId << " to mediaimagecontainer table.";
        linkMediaImagesWithContainer(fileId, images.values());
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

QString DbMediaImageContainer::constructSelect(QString whereClause) const
{
    // TODO, for a usual search we need a "light" version of this select
    // and MediaImageContainer (only id, name)
    QString select = QString("SELECT file.id, file.name, file.checksum, file.size, "
                "        filepath.id, filepath.name, "
                "        setup.id, "
                "        platform.id, platform.name, "
                "        mediatype.id, mediatype.name "
                "FROM file "
                "INNER JOIN mediaimagecontainer_filepath ON mediaimagecontainer_filepath.fileid = file.id "
                "INNER JOIN filepath ON mediaimagecontainer_filepath.filepathid = filepath.id "
                "INNER JOIN setup ON filepath.setupid = setup.id "
                "INNER JOIN platform ON setup.platformid = platform.id "
                "INNER JOIN mediatype ON setup.mediatypeid = mediatype.id "
                "%1 "
                "ORDER BY file.name").arg(whereClause);
    qDebug() << select;
    return select;
}

QString DbMediaImageContainer::constructFilterById(int id) const
{
    return QString("file.id = %1").arg(id);
}

QString DbMediaImageContainer::constructSelectById(int id) const
{
    return constructSelect(
        QString("WHERE %1").arg(constructFilterById(id))
        );
}

EmuFrontObject* DbMediaImageContainer::recordToDataObject(const QSqlRecord *rec)
{
    // TODO: checks!
    MediaImageContainer *mic = 0;
    if (!rec) return mic;
    int id = rec->value(MIC_FileId).toInt();
    QString name = rec->value(MIC_FileName).toString();
    QString checksum = rec->value(MIC_FileCheckSum).toString();
    int size = rec->value(MIC_FileSize).toInt();
    int fpId = rec->value(MIC_FilePathId).toInt();
    FilePathObject *fpo
        = dynamic_cast<FilePathObject*>(dbFilePath->getDataObject(fpId));
    //int supId = rec->value(MIC_SetupId).toInt();
    //Setup *sup = dbSetup->getDataObject(supId)
    QMap<QString, EmuFrontObject*> images = dbMediaImage->getMediaImages(id);

    mic = new MediaImageContainer(
       id, name, checksum, size, images, fpo
    );
    return mic;
}

QSqlQueryModel* DbMediaImageContainer::getData()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    if (sqlTableModel){
        model->setQuery(sqlTableModel->query());
    }
    else
        model->setQuery(constructSelect());
    model->setHeaderData(MIC_FileId, Qt::Horizontal, tr("File id"));
    model->setHeaderData(MIC_FileName, Qt::Horizontal, tr("File Name"));
    model->setHeaderData(MIC_FileCheckSum, Qt::Horizontal, tr("File checksum"));
    model->setHeaderData(MIC_FileSize, Qt::Horizontal, tr("File Size"));
    model->setHeaderData(MIC_FilePathId, Qt::Horizontal, tr("File path id"));
    model->setHeaderData(MIC_FilePathName, Qt::Horizontal, tr("File path name"));
    model->setHeaderData(MIC_SetupId, Qt::Horizontal, tr("Setup id"));
    model->setHeaderData(MIC_PlatformId, Qt::Horizontal, tr("Platform id"));
    model->setHeaderData(MIC_PlatformName, Qt::Horizontal, tr("Platform name"));
    model->setHeaderData(MIC_MediaTypeId, Qt::Horizontal, tr("Media type id"));
    model->setHeaderData(MIC_MediaTypeName, Qt::Horizontal, tr("Media type name"));
    return model;
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
        int micFileId = storeMediaImageContainer(mic);
    }
}

void DbMediaImageContainer::linkMediaImagesWithContainer(int micId, QList<EmuFrontObject*> mediaImages)
{
    if (micId < 0 || mediaImages.count() <= 0)
        return;

    MediaImage *mi = 0;
    foreach(EmuFrontObject *efo, mediaImages) {
        mi = dynamic_cast<MediaImage*>(efo);
        qDebug() << "Linking media image container " << micId
            << " to media image " << mi->getId()  << ", " << mi->getName() << ".";
        if (!linkMediaImageToMediaImageContainer(mi, micId)) {
                throw new EmuFrontException(QString("Failed linking media "
                                                    "image container %1 to a media image %2").arg(micId).arg(mi->getId()));
        }
    }
}

void DbMediaImageContainer::filter(int mediaTypeId, int platformId)
{
    qDebug() << "Filtering media images with media type " << mediaTypeId
        << " and platform " << platformId;
    QList<QString> filters;
    if (mediaTypeId >= 0)
        filters.append(QString("mediatype.id=%1").arg(mediaTypeId));
    if (platformId >= 0)
        filters.append(QString("platform.id=%1").arg(platformId));
    filterDataObjects(filters);
}

QString DbMediaImageContainer::getCountRefsSelect(int id) const
{
    /* we need to count file references to give media image container */
    /* example:
        select count(*) from mediaimagecontainer
        INNER JOIN mediaimagecontainer_mediaimage
        ON mediaimagecontainer_mediaimage.mediaimagecontainerid
        = mediaimagecontainer.fileid
        WHERE mediaimagecontainer.fileid=589;
    */
    return QString("SELECT count(*) FROM mediaimagecontainer "
              "INNER JOIN mediaimagecontainer_mediaimage "
              "ON mediaimagecontainer_mediaimage.mediaimagecontainerid "
              "    =mediaimagecontainer.fileid "
              "WHERE mediaimagecontainer.fileid=%1").arg(id);
}

QString DbMediaImageContainer::getDeleteObjectSql() const
{
       // The trigger will take care of deleting
       // the reference from the mediaimagecontainer
       // and mediaimage_mediaimagecontainer tables.
       // there is also a trigger that will delete
       // all the files linked to mediaimagecontainer
       // using mediaimage_mediaimagecontainer (the actual
       // mediaimages).
       return QString("DELETE FROM file WHERE id=:id");
}

EmuFrontObject* DbMediaImageContainer::getMediaImageContainerByChecksum(QString checksum)
{
    return getDataObject(QString("file.checksum LIKE '%1'").arg(checksum));
}

bool DbMediaImageContainer::linkMediaImageContainerToPath(const MediaImageContainer *mic) const
{
    QSqlQuery q;
    q.prepare("INSERT INTO mediaimagecontainer_filepath "
              "(fileid, filepathid, updatetime) "
              "VALUES (:fileid, :filepathid, :updatetime)");
    q.bindValue(":fileid", mic->getId());
    q.bindValue(":filepathid", mic->getFilePath()->getId());
    q.bindValue(":updatetime", DatabaseManager::getCurrentTimeStamp());
    return q.exec();
}

bool DbMediaImageContainer::linkMediaImageToMediaImageContainer(const MediaImage *mi, int micId) const
{
    QSqlQuery q;
    q.prepare("INSERT INTO mediaimagecontainer_mediaimage "
        "(mediaimagecontainerid, mediaimageid) "
        "VALUES (:micid, :miid) ");
    q.bindValue(":micid", micId);
    q.bindValue(":miid", mi->getId());
    return q.exec();
}

bool DbMediaImageContainer::removeFromFilePath(int filePathId) const
{
    QSqlQuery q;
    q.prepare("DELETE FROM mediaimagecontainer_filepath "
        "WHERE filepathid=:filepathid");
    q.bindValue(":filepathid", filePathId);
    return q.exec();
}

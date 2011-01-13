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

#include "mediaimagecontainer.h"
#include "mediaimagecontainermodel.h"
#include "filepathmodel.h"
#include "mediaimagemodel.h"
#include "emufrontexception.h"
#include <QtSql>

MediaImageContainerModel::MediaImageContainerModel(QObject *parent) :
    FileModel(parent)
{
}

QString MediaImageContainerModel::constructSelect(QString where) const
{
    // TODO, for a usual search need a "light" version of this select
    // and MediaImageContainer (only id, name)
    return QString(
        "SELECT file.id, file.name, file.checksum, file.size, "
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
        "ORDER BY file.name"
    ).arg(where);
}

QString MediaImageContainerModel::constructFilterById(int id) const
{
    return QString("file.id = %1").arg(id);
}

void MediaImageContainerModel::refresh()
{
    //setQuery(constructSelect());
    setHeaderData(MIC_FileId, Qt::Horizontal, tr("File id"));
    setHeaderData(MIC_FileName, Qt::Horizontal, tr("File Name"));
    setHeaderData(MIC_FileCheckSum, Qt::Horizontal, tr("File checksum"));
    setHeaderData(MIC_FileSize, Qt::Horizontal, tr("File Size"));
    setHeaderData(MIC_FilePathId, Qt::Horizontal, tr("File path id"));
    setHeaderData(MIC_FilePathName, Qt::Horizontal, tr("File path name"));
    setHeaderData(MIC_SetupId, Qt::Horizontal, tr("Setup id"));
    setHeaderData(MIC_PlatformId, Qt::Horizontal, tr("Platform id"));
    setHeaderData(MIC_PlatformName, Qt::Horizontal, tr("Platform name"));
    setHeaderData(MIC_MediaTypeId, Qt::Horizontal, tr("Media type id"));
    setHeaderData(MIC_MediaTypeName, Qt::Horizontal, tr("Media type name"));
}

EmuFrontObject* MediaImageContainerModel::recordToDataObject(const QSqlRecord *rec)
{
    // TODO: checks!
    MediaImageContainer *mic = 0;
    if (!rec) return mic;
    int id = rec->value(MIC_FileId).toInt();
    QString name = rec->value(MIC_FileName).toString();
    QString checksum = rec->value(MIC_FileCheckSum).toString();
    int size = rec->value(MIC_FileSize).toInt();
    int fpId = rec->value(MIC_FilePathId).toInt();
    FilePathModel fpModel;
    EmuFrontObject *efo = fpModel.getDataObject(fpId);
    FilePathObject *fpo = dynamic_cast<FilePathObject*>(efo);
    if (!fpo) return 0;
    //int supId = rec->value(MIC_SetupId).toInt();
    //Setup *sup = dbSetup->getDataObject(supId)
    MediaImageModel miModel;
    QMap<QString, EmuFrontObject*> images = miModel.getMediaImages(id);
    mic = new MediaImageContainer(
       id, name, checksum, size, images, fpo
    );
    return mic;
}

void MediaImageContainerModel::filterBySetup(int setupId)
{
    QList<QString> filters;
    filters.append(QString("setup.id = %1").arg(setupId));
    filterDataObjects(filters);
}

bool MediaImageContainerModel::removeFromFilePath(int filePathId) const
{
    QSqlQuery q;
    q.prepare("DELETE FROM mediaimagecontainer_filepath "
        "WHERE filepathid=:filepathid");
    q.bindValue(":filepathid", filePathId);
    return q.exec();
}

void MediaImageContainerModel::storeContainers(QList<MediaImageContainer *> lst, FilePathObject *fpo)
{
    foreach(MediaImageContainer *mic, lst) {
        int micFileId = storeMediaImageContainer(mic);
    }
}

int MediaImageContainerModel::storeMediaImageContainer(EmuFrontObject *efo)
{
    MediaImageContainer *mic
        = dynamic_cast<MediaImageContainer *>(efo);

    if (!mic->getFilePath())
        throw EmuFrontException("Cannot install media image "
            "container to database without a file path object!");

    // multiple media image containers with matching checksum will be stored
    //       if each instance is in a different file path

    int fileId = -1;
    QMap<QString, EmuFrontObject*> images = mic->getMediaImages();
    MediaImageModel miModel;
    QList<int> ids = miModel.storeMediaImages(images);

    //qDebug() << "Stored " << ids.count() << " media images.";

    if (ids.count() <= 0)
        return -1;

    /* Contained Media images successfully stored to db,
        storing media image container also */

    try {

        // Insert MediaImageContainer first as a EmuFrontFile object to file table.

        // File id is used to store the media image container instance to database,
        // file id is also the media image container id

        // TODO: if this fails, the remove the media images in ids
        fileId = insertDataObject(mic);

        //qDebug() << "Inserted media image container to file table with id " << fileId << ".";

        if (fileId < 0) {
            // TODO: note we most surely need to catch the exception
            // in the calling code block and clean
            // all the media image and ...containers from
            // the memory!
            throw EmuFrontException(
                    QString(tr("Inserting media image container %1 to file database failed"))
                    .arg(mic->getName()));
        }

        mic->setId(fileId);

        if (!linkMediaImageContainerToPath(mic)){
            deleteDataObject(fileId);
            throw EmuFrontException("Failed inserting media image to database!");
        }
        //qDebug() << "Inserted media image container " << fileId << " to mediaimagecontainer table.";
        linkMediaImagesWithContainer(fileId, images.values());
        //qDebug() << "Linked media image container with media images.";
    } catch (EmuFrontException e) {
        miModel.removeOrphanedMediaImages(ids);
        throw e;
    }

    return fileId;
}

/* Throws EmuFrontException */
void MediaImageContainerModel::linkMediaImagesWithContainer(int micId, QList<EmuFrontObject*> mediaImages)
{
    if (micId < 0 || mediaImages.count() <= 0)
        return;

    MediaImage *mi = 0;
    foreach(EmuFrontObject *efo, mediaImages) {
        mi = dynamic_cast<MediaImage*>(efo);
        /*qDebug() << "Linking media image container " << micId
            << " to media image " << mi->getId()  << ", " << mi->getName() << ".";*/
        if (!linkMediaImageToMediaImageContainer(mi, micId)) {
                throw EmuFrontException(QString("Failed linking media "
                                                    "image container %1 to a media image %2").arg(micId).arg(mi->getId()));
        }
    }
}

bool MediaImageContainerModel::linkMediaImageContainerToPath(const MediaImageContainer *mic) const
{
    QSqlQuery q;
    q.prepare("INSERT INTO mediaimagecontainer_filepath "
              "(fileid, filepathid, updatetime) "
              "VALUES (:fileid, :filepathid, :updatetime)");
    q.bindValue(":fileid", mic->getId());
    q.bindValue(":filepathid", mic->getFilePath()->getId());
    q.bindValue(":updatetime", getCurrentTimeStamp());
    return q.exec();
}

bool MediaImageContainerModel::linkMediaImageToMediaImageContainer(const MediaImage *mi, int micId) const
{
    QSqlQuery q;
    q.prepare("INSERT INTO mediaimagecontainer_mediaimage "
        "(mediaimagecontainerid, mediaimageid) "
        "VALUES (:micid, :miid) ");
    q.bindValue(":micid", micId);
    q.bindValue(":miid", mi->getId());
    return q.exec();
}


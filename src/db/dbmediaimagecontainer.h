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

#ifndef DBMEDIAIMAGECONTAINER_H
#define DBMEDIAIMAGECONTAINER_H

#include "dbmediaimage.h"
#include "dbfile.h"
#include "../dataobjects/mediaimagecontainer.h"
#include "../dataobjects/filepathobject.h"

class DbMediaImageContainer;
class DbMediaImage;
class DbFilePath;
class DbSetup;

class DbMediaImageContainer : public DbFile // DbQueryModelManager
{
public:
    DbMediaImageContainer(QObject *parent);
    virtual bool updateDataObjectToModel(const EmuFrontObject *);
    virtual bool deleteDataObjectFromModel(QModelIndex *);
    void storeContainers(QList<MediaImageContainer*>, FilePathObject*);
    int getMediaImageContainer(QString checksum) const;
    void filter(int mediaTypeId, int platformId);
    bool removeFromFilePath(int filePathId) const;
   enum {
        MIC_FileId = 0,
        MIC_FileName,
        MIC_FileCheckSum,
        MIC_FileSize,
        MIC_FilePathId,
        MIC_FilePathName,
        MIC_SetupId,
        MIC_PlatformId,
        MIC_PlatformName,
        MIC_MediaTypeId,
        MIC_MediaTypeName
   };

protected:
    virtual QString constructSelect(QString whereClause = "") const;
    virtual QString constructSelectById(int id) const;
    virtual QString constructFilterById(int id) const;
    virtual EmuFrontObject* recordToDataObject(const QSqlRecord *);
    EmuFrontObject* getMediaImageContainerByChecksum(QString checksum);
private:
    virtual QSqlQueryModel* getData();
    int storeMediaImageContainer(EmuFrontObject *efo);
    void linkMediaImagesWithContainer(int, QList<EmuFrontObject*>);
    bool linkMediaImageContainerToPath(const MediaImageContainer*) const;
    bool linkMediaImageToMediaImageContainer(const MediaImage*, int micId) const;
    QString getMediaImageContainerName(int filePathId, int micId) const;
    DbMediaImage *dbMediaImage;
    DbFilePath *dbFilePath;
    virtual QString getCountRefsSelect(int) const;
    virtual QString getDeleteObjectSql() const;
};

#endif // DBMEDIAIMAGECONTAINER_H

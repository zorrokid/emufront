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

#ifndef DBMEDIAIMAGECONTAINER_H
#define DBMEDIAIMAGECONTAINER_H

#include "dbquerymodelmanager.h"
#include "dbmediaimage.h"
#include "../dataobjects/mediaimagecontainer.h"
#include "../dataobjects/filepathobject.h"

class DbMediaImageContainer : public DbQueryModelManager
{
public:
    DbMediaImageContainer(QObject *parent);
    virtual bool updateDataObjectToModel(const EmuFrontObject *);
    virtual int insertDataObjectToModel(const EmuFrontObject *);
    virtual bool deleteDataObjectFromModel(QModelIndex *);
    virtual int countDataObjectRefs(int id) const;
    void storeContainers(QList<MediaImageContainer*>, FilePathObject*);
    int getMediaImageContainer(QString checksum) const;
protected:
    virtual QString constructSelect(QString whereClause) const;
    virtual QString constructSelectById(int id) const;
    virtual QString constructFilterById(int id) const;
    virtual EmuFrontObject* recordToDataObject(const QSqlRecord *);
private:
    virtual QSqlQueryModel* getData();
    void linkMediaImagesWithContainer(int, QList<int>);
    DbMediaImage *dbMediaImage;
 };

#endif // DBMEDIAIMAGECONTAINER_H

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

#ifndef DBMEDIAIMAGE_H
#define DBMEDIAIMAGE_H

#include "dbfile.h"
#include "../dataobjects/mediaimage.h"

class DbMediaImage : public DbFile
{
public:
    DbMediaImage(QObject *parent);
    QList<int> storeMediaImages(QList<MediaImage*>);
    void removeOrphanedMediaImages(QList<int> ids);
    /*virtual bool updateDataObjectToModel(const EmuFrontObject *);
    virtual bool insertDataObjectToModel(const EmuFrontObject *);
    virtual bool deleteDataObjectFromModel(QModelIndex *);
    virtual int countDataObjectRefs(int id) const;
    int getMediaImage(QString checksum) const;
    int insertMediaImage(const MediaImage*);
protected:
    virtual QString constructSelect(QString whereClause) const;
    virtual QString constructSelectById(int id) const;
    virtual EmuFrontObject* recordToDataObject(const QSqlRecord *);
private:
    virtual QSqlQueryModel* getData();*/
};

#endif // DBMEDIAIMAGE_H

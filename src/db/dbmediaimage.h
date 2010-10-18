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

#ifndef DBMEDIAIMAGE_H
#define DBMEDIAIMAGE_H

#include "dbfile.h"
#include "../dataobjects/mediaimage.h"
#include <QMap>

class DbMediaImage : public DbFile
{
public:
    DbMediaImage(QObject *parent);
    QList<int> storeMediaImages(QMap<QString, EmuFrontObject*>);
    void removeOrphanedMediaImages(QList<int> ids);
    QMap<QString, EmuFrontObject*> getMediaImages(int id) const;
    /*virtual bool updateDataObjectToModel(const EmuFrontObject *);
    virtual bool insertDataObjectToModel(const EmuFrontObject *);
    virtual bool deleteDataObjectFromModel(QModelIndex *);
    int getMediaImage(QString checksum) const;
    int insertMediaImage(const MediaImage*);
protected:
    virtual QString constructSelect(QString whereClause) const;
    virtual QString constructSelectById(int id) const;
    virtual EmuFrontObject* recordToDataObject(const QSqlRecord *);
    */
private:
    /*virtual QSqlQueryModel* getData();*/
    virtual QString getCountRefsSelect(int) const;
};

#endif // DBMEDIAIMAGE_H

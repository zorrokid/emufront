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

#include "dbmediaimagecontainer.h"

DbMediaImageContainer::DbMediaImageContainer(QObject *parent)
    : DbQueryModelManager(parent)
{
    dbMediaImage = 0;
}

bool DbMediaImageContainer::updateDataObjectToModel(const EmuFrontObject *efo)
{
    // TODO
    return false;
}

bool DbMediaImageContainer::insertDataObjectToModel(const EmuFrontObject *efo)
{
    // TODO
    return false;
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

void DbMediaImageContainer::storeContainers(QList<MediaImageContainer *> lst)
{
    foreach(MediaImageContainer *mic, lst)
    {
        QList<MediaImage*> images = mic->getMediaImages();
        if (getMediaImageContainer(mic->getCheckSum()) >= 0)
            continue;
        // this is a new media image container, lets build a list
        // of media image id's for this container
        QList<int> ids = dbMediaImage->storeMediaImages(images);

        if (ids.count() > 0)
        {
            // store media image to id

            // get last insert id of stored media image

            // link all the ids in ids-list to media image id
        }
    }
}

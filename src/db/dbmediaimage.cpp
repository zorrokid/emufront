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
#include "dbmediaimage.h"

DbMediaImage::DbMediaImage(QObject *parent)
    : DbQueryModelManager(parent)
{ }

bool DbMediaImage::updateDataObjectToModel(const EmuFrontObject *efo)
{
    return false;
}

bool DbMediaImage::insertDataObjectToModel(const EmuFrontObject *efo)
{
    return false;
}

bool DbMediaImage::deleteDataObjectFromModel(QModelIndex *i)
{
    return false;
}

int DbMediaImage::countDataObjectRefs(int id) const
{
    return -1;
}

QString DbMediaImage::constructSelect(QString whereClause) const
{
    return "";
}

QString DbMediaImage::constructSelectById(int id) const
{
    return "";
}

EmuFrontObject* DbMediaImage::recordToDataObject(const QSqlRecord *)
{
    return 0;
}

QSqlQueryModel* DbMediaImage::getData()
{
    return 0;
}

int DbMediaImage::getMediaImage(QString checksum) const
{
    return -1;

}

int DbMediaImage::insertMediaImage(const MediaImage *mi)
{
    return -1;
}

QList<int> DbMediaImage::storeMediaImages(QList<MediaImage *> images)
{
    QList<int> ids  = QList<int>();
    foreach(MediaImage* mi, images)
    {
        QString cksum = mi->getCheckSum();
        int id = getMediaImage(cksum);
        if (id >= 0)
        {
            // this media image is already in the database
            // TODO: what if the name differs?
            ids.append(id);
            continue;
        }
        if (id < 0)
        {
            // insert new media image to db
            id = insertMediaImage(mi);
            if (id < 0)
            {
                // TODO: Build an error message of failed inserts
                qDebug() << "Failed inserting media image" << mi->getName();
            }
            else
            {
                ids.append(id);
            }
        }
    }
}

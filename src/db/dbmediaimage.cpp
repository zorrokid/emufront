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
#include <QSqlTableModel>
#include <QSqlQuery>
#include "dbmediaimage.h"

DbMediaImage::DbMediaImage(QObject *parent)
    : DbFile(parent)
{
    type = EmuFrontFile::FileType_MediaImage;
}

/*bool DbMediaImage::updateDataObjectToModel(const EmuFrontObject *efo)
{
    // TODO
    return false;
}*/

/*bool DbMediaImage::insertDataObjectToModel(const EmuFrontObject *efo)
{
    // TODO
    return false;
}*/

/*bool DbMediaImage::deleteDataObjectFromModel(QModelIndex *i)
{
    // TODO
    return false;
}*/

/*int DbMediaImage::countDataObjectRefs(int id) const
{
    // TODO
    return -1;
}*/

/*QString DbMediaImage::constructSelect(QString whereClause) const
{
    // TODO
    return "";
}

QString DbMediaImage::constructSelectById(int id) const
{
    // TODO
    return "";
}*/

/*EmuFrontObject* DbMediaImage::recordToDataObject(const QSqlRecord *)
{
    // TODO
    return 0;
}*/

/*QSqlQueryModel* DbMediaImage::getData()
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable(DB_TABLE_NAME_FILE);
}*/



int DbMediaImage::insertMediaImage(const MediaImage *mi)
{
    return DbFile::insertFile(mi);
}

QList<int> DbMediaImage::storeMediaImages(QList<MediaImage *> images)
{
    QList<int> ids  = QList<int>();
    foreach(MediaImage* mi, images)
    {
        QString cksum = mi->getCheckSum();
        EmuFrontObject *o = getFileByChecksum(cksum);
        int id = o ? o->getId() : -1;
        if (id >= 0)
        {
            // this media image is already in the database
            // TODO: what if the name differs?
        }
        else if (id < 0)
        {
            id = insertMediaImage(mi);
            if (id < 0)
            {
                // TODO: Build an error message of failed inserts
                qDebug() << "Failed inserting media image" << mi->getName();
            }
        }
        ids.append(id);
    }
}

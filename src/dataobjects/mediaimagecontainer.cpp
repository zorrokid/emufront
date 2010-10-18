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
#include "mediaimagecontainer.h"

MediaImageContainer::MediaImageContainer()
    : EmuFrontFile(EmuFrontFile::FileType_MediaImageContainer)
{
    lstMediaImage = QMap<QString, EmuFrontObject*>();
    filePath = 0;
}

MediaImageContainer::MediaImageContainer(int id, QString name, 
    QString checksum, int size, QMap<QString, EmuFrontObject*>images, FilePathObject *fpo)
    : EmuFrontFile(id, name, checksum, size, EmuFrontFile::FileType_MediaImageContainer),
        lstMediaImage(images), filePath(fpo)
{ }

MediaImageContainer::MediaImageContainer(QString name, QString checksum,
    int size, QMap<QString, EmuFrontObject*>images, FilePathObject *fpo)
    : EmuFrontFile(-1, name, checksum, size, EmuFrontFile::FileType_MediaImageContainer),
        lstMediaImage(images), filePath(fpo)
{ }

MediaImageContainer::~MediaImageContainer()
{
    qDeleteAll(lstMediaImage);
    delete filePath;
}

MediaImageContainer::MediaImageContainer(MediaImageContainer &mic)
    : EmuFrontFile(mic)
{
    lstMediaImage = QMap<QString, EmuFrontObject*>();

    QMapIterator<QString, EmuFrontObject*> it(mic.lstMediaImage);
    MediaImage *mi = 0;
    while(it.hasNext()) {
        it.next();
        mi = dynamic_cast<MediaImage*>(it.value());
        lstMediaImage[mi->getCheckSum()] = new MediaImage(*mi);
    }

    filePath = new FilePathObject(*(mic.filePath));
}

MediaImageContainer& MediaImageContainer::operator =(MediaImageContainer &mic)
{
     if (this == &mic) return *this;
    id = mic.id;
    name = mic.name;
    type = mic.type;
    checkSum = mic.checkSum;
    size = mic.size;
    qDeleteAll(lstMediaImage);

    QMapIterator<QString, EmuFrontObject*> it(mic.lstMediaImage);
    MediaImage *mi = 0;
    while(it.hasNext()) {
        it.next();
        mi = dynamic_cast<MediaImage*>(it.value());
        lstMediaImage[mi->getCheckSum()] = new MediaImage(*mi);
    }
    filePath = new FilePathObject(*(mic.filePath));
    return (*this);
}

void MediaImageContainer::setMediaImages(QMap<QString, EmuFrontObject*> list)
{
    qDeleteAll(lstMediaImage);
    lstMediaImage = list;
}

QMap<QString, EmuFrontObject*> MediaImageContainer::getMediaImages() const
{   return lstMediaImage; }

void MediaImageContainer::addMediaImage(MediaImage *mi)
{   lstMediaImage[mi->getCheckSum()] = mi; }

void MediaImageContainer::clearMediaImages()
{
    qDeleteAll(lstMediaImage);
    lstMediaImage.clear();
}

FilePathObject* MediaImageContainer::getFilePath() const
{ return filePath; }

void MediaImageContainer::setFilePath(FilePathObject *fp)
{ filePath = fp; }

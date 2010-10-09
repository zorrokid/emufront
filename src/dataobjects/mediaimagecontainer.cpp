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
#include "mediaimagecontainer.h"

MediaImageContainer::MediaImageContainer()
    : EmuFrontFile(EmuFrontFile::FileType_MediaImageContainer)
{
    lstMediaImage = QList<MediaImage*>();
    filePath = 0;
}

MediaImageContainer::MediaImageContainer(int id, QString name, 
    QString checksum, int size, QList<MediaImage *>images, FilePathObject *fpo)
    : EmuFrontFile(id, name, checksum, size, EmuFrontFile::FileType_MediaImageContainer),
        lstMediaImage(images), filePath(fpo)
{ }

MediaImageContainer::MediaImageContainer(QString name, QString checksum,
    int size, QList<MediaImage *>images, FilePathObject *fpo)
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
    lstMediaImage = QList<MediaImage*>();
    foreach(MediaImage *mi, mic.lstMediaImage)
        lstMediaImage.append(new MediaImage(*mi));
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
    foreach(MediaImage *mi, mic.lstMediaImage)
        lstMediaImage.append(new MediaImage(*mi));
    filePath = new FilePathObject(*(mic.filePath));
    return (*this);
}

void MediaImageContainer::setMediaImages(QList<MediaImage *> list)
{
    qDeleteAll(lstMediaImage);
    lstMediaImage = list;
}

QList<MediaImage *> MediaImageContainer::getMediaImages() const
{   return lstMediaImage; }

void MediaImageContainer::addMediaImage(MediaImage *mi)
{   lstMediaImage.append(mi); }

void MediaImageContainer::clearMediaImages()
{
    qDeleteAll(lstMediaImage);
    lstMediaImage.clear();
}

FilePathObject* MediaImageContainer::getFilePath() const
{ return filePath; }

void MediaImageContainer::setFilePath(FilePathObject *fp)
{ filePath = fp; }

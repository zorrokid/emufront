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

#include "mediaimagecontainer.h"

MediaImageContainer::MediaImageContainer()
    : EmuFrontFile(EmuFrontFile::FileType_MediaImageContainer)
{
    lstMediaImage = QList<MediaImage*>();
}

MediaImageContainer::MediaImageContainer(int id, QString name, QString checksum, int size, QList<MediaImage *>images)
    : EmuFrontFile(id, name, checksum, size, EmuFrontFile::FileType_MediaImageContainer)
{
    lstMediaImage = QList<MediaImage*>();
}

MediaImageContainer::MediaImageContainer(QString name, QString checksum, int size, QList<MediaImage *>images)
    : EmuFrontFile(-1, name, checksum, size, EmuFrontFile::FileType_MediaImageContainer)
{
    lstMediaImage = QList<MediaImage*>();
}

MediaImageContainer::~MediaImageContainer()
{
    qDeleteAll(lstMediaImage);
}

MediaImageContainer::MediaImageContainer(MediaImageContainer &mic)
    : EmuFrontFile(mic.id, mic.name, mic.checkSum, mic.size, mic.type)
{
    lstMediaImage = QList<MediaImage*>();
    foreach(MediaImage *mi, mic.lstMediaImage)
        lstMediaImage.append(new MediaImage(mi));
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
    foreach(MediaImage *mi, mic)
        lstMediaImage.append(new MediaImage(mi));
    return (*this);
}

void MediaImageContainer::setMediaImages(QList<MediaImage *> list)
{
    qDeleteAll(lstMediaImage);
    lstMediaImage = list;
}

void MediaImageContainer::addMediaImage(MediaImage *mi)
{   lstMediaImage.append(mi); }

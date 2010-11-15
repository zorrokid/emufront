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
#include "setup.h"
#include "platform.h"
#include "mediatype.h"

Setup::Setup() : EmuFrontObject(), platform(0), mediaType(0)
{
}

Setup::Setup(int id, Platform *plf, MediaType *mt, QStringList fileTypeExtensions)
    : EmuFrontObject(id,
        QString("%1%2")
            .arg(plf ? plf->getName() : "")
            .arg(mt ? mt->getName() : "")), platform(plf), mediaType(mt),
            fileTypeExtensions(fileTypeExtensions){

}

Setup::~Setup()
{
    delete platform;
    delete mediaType;
}

Setup::Setup(const Setup &s)
        : EmuFrontObject(s),
            fileTypeExtensions(s.fileTypeExtensions)
{
    Platform *p = s.platform;
    MediaType *m = s.mediaType;
    platform = new Platform(*p);
    mediaType = new MediaType(*m);
}

Setup& Setup::operator =(const Setup &sup)
{
    if (this == &sup) return *this;
    id = sup.id;
    name = sup.name;
    if (platform) delete platform;
    Platform *p = sup.platform;
    MediaType *m = sup.mediaType;
    platform = new Platform(*p);
    if (mediaType) delete mediaType;
    mediaType = new MediaType(*m);
    return (*this);
}

// TODO:
/*bool Setup::operator ==(const Setup &sup)
{

    // TODO: more precise ... this is exact copy from EmuFrontObject
    return (id >= 0 && id == sup.id);

}

bool Setup::operator !=(const Setup &sup)
{
    // TODO: more precise ... this is exact copy from EmuFrontObject
    return !(*this == sup);
}*/

Platform* Setup::getPlatform() const
{ return platform; }
void Setup::setPlatform(Platform *plf)
{ platform = plf; }

MediaType* Setup::getMediaType() const
{ return mediaType; }
void Setup::setMediaType(MediaType *mt)
{ mediaType = mt; }

QStringList Setup::getSupportedFileTypeExtensions() const
{ return fileTypeExtensions; }
void Setup::setSupportedFileTypeExtensions(QStringList ftx)
{ fileTypeExtensions = ftx; }

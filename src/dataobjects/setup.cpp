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
        : EmuFrontObject(s.getId(), s.getName()),
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

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
// Foobar is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#ifndef FILEPATHOBJECT_H
#define FILEPATHOBJECT_H

#include "emufrontfileobject.h"
#include "platform.h"
#include "mediatype.h"

class FilePathObject : public EmuFrontFileObject
{
public:
    FilePathObject();
    ~FilePathObject();
    FilePathObject(int id, QString name, QString filename, int filetype);
    FilePathObject(int id, QString name, QString filename, int filetype, Platform*, MediaType*);
    FilePathObject(const FilePathObject &);
    FilePathObject& operator=(const FilePathObject &);

    Platform* getPlatform() const
    { return platform; }
    MediaType* getMediaType() const
    { return mediaType; }
    void setPlatform(Platform *plf)
    { platform = plf; }
    void setMediaType(MediaType *med)
    { mediaType = med; }

private:
    Platform *platform;
    MediaType *mediaType;

};

#endif // FILEPATHOBJECT_H

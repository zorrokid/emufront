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

#ifndef MEDIAIMAGECONTAINER_H
#define MEDIAIMAGECONTAINER_H

#include "emufrontfileobject.h"
#include "mediaimage.h"
#include "setup.h"
#include "filepathobject.h"

class MediaImageContainer : public EmuFrontFile
{
public:
    MediaImageContainer();
    MediaImageContainer(int id, QString name, QString checksum,
        int size, QList<MediaImage*> images/*, Setup *setup*/, FilePathObject *fpo);
    MediaImageContainer(QString name, QString checksum,
        int size, QList<MediaImage*> images/*, Setup *setup*/, FilePathObject *fpo);
    ~MediaImageContainer();
    MediaImageContainer(MediaImageContainer&);
    MediaImageContainer& operator=(MediaImageContainer&);
    QList<MediaImage*> getMediaImages() const;
    void setMediaImages(QList<MediaImage*>);
    void addMediaImage(MediaImage*);
    /*void setSetup(Setup *);
    Setup* getSetup() const;*/
    void setFilePath(FilePathObject*);
    FilePathObject* getFilePath() const;
private:
    QList<MediaImage*> lstMediaImage;
    //Setup *setup;
    FilePathObject *filePath;
};

#endif // MEDIAIMAGECONTAINER_H

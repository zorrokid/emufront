/*
** EmuFront
** Copyright 2010 Mikko Keinänen
**
** This file is part of EmuFront.
**
**
** EmuFront is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License version 2 as published by
** the Free Software Foundation and appearing in the file gpl.txt included in the
** packaging of this file.
**
** EmuFront is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.
*/#ifndef MEDIAIMAGECONTAINER_H
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
        int size, QMap<QString, EmuFrontObject*> images, FilePathObject *fpo);
    MediaImageContainer(QString name, QString checksum,
        int size, QMap<QString, EmuFrontObject*> images, FilePathObject *fpo);
    ~MediaImageContainer();
    MediaImageContainer(MediaImageContainer&);
    MediaImageContainer& operator=(MediaImageContainer&);
    QMap<QString, EmuFrontObject*> getMediaImages() const;
    void setMediaImages(QMap<QString, EmuFrontObject*>);
    void addMediaImage(MediaImage*);
    void clearMediaImages();
    void setFilePath(FilePathObject*);
    FilePathObject* getFilePath() const;
private:
    QMap<QString, EmuFrontObject*> lstMediaImage;
    FilePathObject *filePath;
};

#endif // MEDIAIMAGECONTAINER_H

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
*/



#ifndef MEDIAIMAGE_H
#define MEDIAIMAGE_H

#include "emufrontfileobject.h"

/* MediaImage is a EmuFrontFile of type  FileType_MediaImage */
class MediaImage : public EmuFrontFile
{
public:
    MediaImage();
    MediaImage(QString name, QString checksum, int size);
    MediaImage(int id, QString name, QString checksum, int size);
    MediaImage(MediaImage&);
};

#endif // MEDIAIMAGE_H

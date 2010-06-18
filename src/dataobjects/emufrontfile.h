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

#ifndef EMUFRONTFILE_H
#define EMUFRONTFILE_H

#include "emufrontobject.h"

class EmuFrontFile : public EmuFrontObject
{
public:
    EmuFrontFile();
    EmuFrontFile(int id, QString name, QString checksum, int size, int type);
    QString getCheckSum() const;
    void setCheckSum(QString);
    int getSize() const;
    void setSize(int);
    void setType(int);
    int getType() const;

    enum {
        FileType_MediaImageContainerDir = 1,
        FileType_MediaImageContainer,
        FileType_MediaImage,
        FileType_ScreenShot,
        FileType_PlatformIconPath,
        FileType_MediaTypeIconPath
      };

protected:
    QString checkSum;
    int size;
    int type;
};

#endif // EMUFRONTFILE_H

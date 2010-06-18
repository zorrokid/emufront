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

class MediaImageContainer : public EmuFrontFileObject
{
public:
    MediaImageContainer();
    QList<MediaImage*> getMediaImages();
    void setMediaImages(QList<MediaImage*>);
    void addMediaImage(MediaImage*);
private:
    QList<MediaImage*> lstMediaImage;
};

#endif // MEDIAIMAGECONTAINER_H

/*
** EmuFront
** Copyright 2010-2011 Mikko Keinänen
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

#ifndef SETUP_H
#define SETUP_H
#include <QStringList>
#include "emufrontobject.h"

class Platform;
class MediaType;

class Setup : public EmuFrontObject
{
public:
    Setup();
    ~Setup();
    Setup(int id, Platform *, MediaType *, QStringList fileTypeExtensions);
    Setup(const Setup &);
    Setup& operator=(const Setup &);
    // TODO:
    //bool operator==(const Setup &);
    //bool operator!=(const Setup &);
    Platform* getPlatform() const;
    MediaType* getMediaType() const;
    QStringList getSupportedFileTypeExtensions() const;
    void setPlatform(Platform *);
    void setMediaType(MediaType *);
    void setSupportedFileTypeExtensions(QStringList);


private:
    Platform *platform;
    MediaType *mediaType;
    // QStringList is implicitly shared.
    QStringList fileTypeExtensions;
};

#endif // SETUP_H

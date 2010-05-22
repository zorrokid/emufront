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

#ifndef PLATFORM_H
#define PLATFORM_H

#include "emufrontobject.h"

class Platform : public EmuFrontObject
{
public:
    Platform();
    Platform(int id, QString name, QString filename);
    // No need for these as long we use QString (see Implicit Data Sharing)
    /*Platform(const Platform &);
    Platform &operator=(const Platform &);
    virtual ~Platform();*/
    const QString getFilename() const
    { return filename; }
    void setFilename(QString filename)
    { this->filename = filename; }

private:
    QString filename;
};

#endif // PLATFORM_H

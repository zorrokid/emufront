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

#ifndef EMUFRONTFILEOBJECT_H
#define EMUFRONTFILEOBJECT_H

#include "emufrontobject.h"
#include "emufrontfile.h"

class EmuFrontFileObject : public EmuFrontObject
{
public:
    EmuFrontFileObject();
    EmuFrontFileObject(int id, QString name, EmuFrontFile *file);
    EmuFrontFileObject(const EmuFrontFileObject&);
    EmuFrontFileObject& operator=(const EmuFrontFileObject&);
    ~EmuFrontFileObject();

    EmuFrontFile* getFile() const
    { return file; }
    void setFile(EmuFrontFile *file)
    { this->file = file; }

protected:
    EmuFrontFile *file;
};

#endif // EMUFRONTFILEOBJECT_H

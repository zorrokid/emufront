// EmuFront
// Copyright 2010 Mikko Keinänen
//
// This file is part of EmuFront.
//
//
// EmuFront is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation and appearing in the file gpl.txt included in the
// packaging of this file.
//
// EmuFront is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#include "emufrontfileobject.h"

EmuFrontFileObject::EmuFrontFileObject()
    : EmuFrontObject(-1, ""), file(0) { }

EmuFrontFileObject::EmuFrontFileObject(int id, QString name, EmuFrontFile *file)
    : EmuFrontObject(id, name), file(file) {}

EmuFrontFileObject::EmuFrontFileObject(const EmuFrontFileObject &pl)
    : EmuFrontObject(pl)
{
    EmuFrontFile *f = pl.file;
    file = f ? new EmuFrontFile(*f) : 0;
}

EmuFrontFileObject::~EmuFrontFileObject()
{
    if (file) delete file;
}

EmuFrontFileObject& EmuFrontFileObject::operator =(const EmuFrontFileObject &ob)
{
    if (this == &ob) return (*this);
    id = ob.id;
    name = ob.name;
    if (file) delete file;
    EmuFrontFile *f = ob.file;
    file = new EmuFrontFile(*f);
    return (*this);
}

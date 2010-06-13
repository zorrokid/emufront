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

#include "filepathobject.h"
#include "setup.h"

FilePathObject::FilePathObject() : EmuFrontObject(), setup(0)
{ }

FilePathObject::FilePathObject(int id, QString name, int filetype)
    : EmuFrontObject(id, name), type(filetype), setup(0) {}

 FilePathObject::FilePathObject(int id, QString name, int filetype, Setup *setup)
    : EmuFrontObject(id, name), type(filetype), setup(setup) {}

FilePathObject::~FilePathObject()
{
    if (setup) delete setup;
}

FilePathObject::FilePathObject(const FilePathObject &fpobj)
    : EmuFrontObject(fpobj.id, fpobj.name), type(fpobj.type)
{
    Setup *s = fpobj.setup;
    setup = new Setup(*s);
}

FilePathObject& FilePathObject::operator =(const FilePathObject &fpobj)
{
    if (this == &fpobj) return *this;
    id = fpobj.id;
    name = fpobj.name;
    type = fpobj.type;
    if (setup) delete setup;
    Setup *sup = fpobj.setup;
    setup = new Setup(*sup);
    return (*this);
}

Setup* FilePathObject::getSetup() const
{ return setup; }
void FilePathObject::setSetup(Setup *sup)
{ setup = sup; }

int FilePathObject::getType() const
{ return type; }
void FilePathObject::setType(int t)
{ type = t; }

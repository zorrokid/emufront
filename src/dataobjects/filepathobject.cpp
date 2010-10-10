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

#include <QDebug>
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
    : EmuFrontObject(fpobj), type(fpobj.type)
{
    qDebug() << "FilePathObject copy constructor.";
    Setup *s = fpobj.setup;
    qDebug() << "Setup name " << s->getName();
    qDebug() << "Setup id " << s->getId();
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

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

#include "emufrontfileobject.h"

EmuFrontFileObject::EmuFrontFileObject() : EmuFrontObject(-1, ""), filename(""), filetype(-1)
{ }

EmuFrontFileObject::EmuFrontFileObject(int id, QString name, QString filename)
    : EmuFrontObject(id, name), filename(filename), filetype(-1)
{}

EmuFrontFileObject::EmuFrontFileObject(int id, QString name, QString filename, int filetype)
    : EmuFrontObject(id, name), filename(filename), filetype(filetype)
{}


/*EmuFrontFileObject::EmuFrontFileObject(const EmuFrontFileObject&pl)
    : EmuFrontObject(pl.id, pl.name), filename(pl.filename)
{
    // no need to perform deep copy here, see:
    // http://doc.trolltech.com/4.0/shclass.html
}*/

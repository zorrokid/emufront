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

#include "emufrontobject.h"

EmuFrontObject::EmuFrontObject() : id(-1), name("")
{
}

EmuFrontObject::EmuFrontObject(const EmuFrontObject &ob)
    : QObject(), id(ob.id), name(ob.name)
{
    // no need to perform deep copy here, see:
    // http://doc.trolltech.com/4.0/shclass.html
}

EmuFrontObject::EmuFrontObject(int id, QString name)
    : id(id), name(name)
{}

EmuFrontObject::~EmuFrontObject()
{
}

EmuFrontObject& EmuFrontObject::operator =(const EmuFrontObject &ob)
{
    if (this == &ob) return (*this);
    id = ob.id;
    name = ob.name;
    return (*this);
}


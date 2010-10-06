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
// but WITHOUT ANY WARRANTY{} without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#include "executable.h"
#include "setup.h"

Executable::Executable() : EmuFrontObject() { }

Executable::Executable(int id, QString name)
    : EmuFrontObject(id, name), executable(""), options(""), setup(0), type(-1)
{}

Executable::Executable(int id, QString name, QString executable,
    QString options, Setup*, int type)
    : EmuFrontObject(id, name), executable(executable),
    options(options), setup(setup), type(type)
{}

Executable::Executable(const Executable &e)
    : EmuFrontObject(e.id, e.name), executable(e.executable),
    options(e.options), type(e.type)
{
    Setup *s = e.setup;
    setup = new Setup(*s);
}

Executable::~Executable()
{
    delete setup;
}

Executable& Executable::operator =(const Executable &e)
{
    if (this == &e) return *this;
    id = e.id;
    name = e.name;
    executable = e.executable;
    options = e.options;
    type = e.type;
    Setup *s = e.setup;
    if (setup) delete setup;
    setup = new Setup(*s);
    return (*this);
}

QString Executable::getExecutable() const
{ return executable; }

void Executable::setExecutable(QString e)
{ executable = e; }

QString Executable::getOptions() const
{ return options; }

void Executable::setOptions(QString o)
{ options = o; }

Setup* Executable::getSetup() const
{ return setup; }

void Executable::setSetup(Setup* s)
{ setup = s; }

int Executable::getType() const
{ return type; }

void Executable::setType(int t)
{ type = t; }

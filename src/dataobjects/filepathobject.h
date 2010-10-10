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

#ifndef FILEPATHOBJECT_H
#define FILEPATHOBJECT_H

#include "emufrontfileobject.h"

class Setup;

class FilePathObject : public EmuFrontObject
{
public:
    FilePathObject();
    ~FilePathObject();
    FilePathObject(int id, QString name, int filetype);
    FilePathObject(int id, QString name, int filetype, Setup*);
    FilePathObject(const FilePathObject &);
    FilePathObject& operator=(const FilePathObject &);

    Setup* getSetup() const;
    void setSetup(Setup *);
    int getType() const;
    void setType(int);

private:
    int type;
    Setup *setup;
};

#endif // FILEPATHOBJECT_H

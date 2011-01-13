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

#ifndef EXECUTABLE_H
#define EXECUTABLE_H

#include "emufrontobject.h"

class Setup;

class Executable : public EmuFrontObject
{
public:
    Executable();
    Executable(int id, QString name);
    Executable(int id, QString name, QString executable,
        QString options, Setup*, int type);
    Executable(const Executable &);
    ~Executable();
    Executable& operator =(const Executable &);
    QString getExecutable() const;
    void setExecutable(QString);
    QString getOptions() const;
    void setOptions(QString);
    Setup* getSetup() const;
    void setSetup(Setup*);
    int getType() const;
    void setType(int);
    enum {
        ExecutableType_Emulator
    };
private:
    QString executable;
    QString options;
    Setup *setup;
    int type;
};

#endif // EXECUTABLE_H

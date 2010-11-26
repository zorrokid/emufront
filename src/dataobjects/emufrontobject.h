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

#ifndef EMUFRONTOBJECT_H
#define EMUFRONTOBJECT_H

#include <QObject>

class EmuFrontObject : public QObject
{
public:
    static const int ID_NOT_SET;
    EmuFrontObject();
    EmuFrontObject(int id, QString name);

    EmuFrontObject(const EmuFrontObject &);
    ~EmuFrontObject();
    EmuFrontObject &operator=(const EmuFrontObject &);
    bool operator==(const EmuFrontObject &);
    bool operator!=(const EmuFrontObject &);

    virtual const QString getName() const
    { return name; }
    virtual int getId() const
    { return id; }
    virtual void setName(QString name)
    { this->name = name; }
    virtual void setId(int id)
    { this->id = id; }

protected:
    int id;
    QString name;
};

#endif // EMUFRONTOBJECT_H

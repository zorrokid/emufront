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

#ifndef MEDIAIMAGEPATHMAINDIALOG_H
#define MEDIAIMAGEPATHMAINDIALOG_H

#include "dbobjectdialog.h"

class MediaImagePathMainDialog : public DbObjectDialog
{
    Q_OBJECT

public:
    MediaImagePathMainDialog(QWidget *parent);
    ~MediaImagePathMainDialog();

protected:
    virtual void deleteCurrentObject();
    virtual void addObject();
    virtual bool deleteItem();
    virtual void updateDb(const EmuFrontObject*) const;
    virtual void insertDb(const EmuFrontObject*) const;

};

#endif // MEDIAIMAGEPATHMAINDIALOG_H

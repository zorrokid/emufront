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

#ifndef MEDIATYPEDIALOG_H
#define MEDIATYPEDIALOG_H

#include "dbobjectdialog.h"

class MediaTypeDialog : public DbObjectDialog
{
    Q_OBJECT

public:
    MediaTypeDialog(QWidget *parent = 0);
    ~MediaTypeDialog();

protected:
    virtual void deleteCurrentObject();
    virtual EmuFrontObject* createObject();
    virtual void initEditDialog();
};

#endif // MEDIATYPEDIALOG_H

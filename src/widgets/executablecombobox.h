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
*/#ifndef EXECUTABLECOMBOBOX_H
#define EXECUTABLECOMBOBOX_H

#include "efcombobox.h"

class ExecutableComboBox : public EFComboBox
{
public:
    ExecutableComboBox(DatabaseManager *dbMan, QWidget *parent);
    void updateToSetup(int platformId, int mediaTypeId);
protected:
    virtual void reset();
private:
    int currentPlatformId;
    int currentMediaTypeId;
};

#endif // EXECUTABLECOMBOBOX_H

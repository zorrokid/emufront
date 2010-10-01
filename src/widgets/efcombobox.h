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
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#ifndef EFCOMBOBOX_H
#define EFCOMBOBOX_H

#include <QComboBox>
#include "../db/databasemanager.h"

class EFComboBox : public QComboBox
{
public:
    EFComboBox(DatabaseManager *dbMan, QWidget *parent = 0);
    ~EFComboBox();
    EmuFrontObject* getSelected() const;
    // This will replace DataObjectEditDialog::setSelected!
    void setSelected(const EmuFrontObject*);
    void updateDataModel();
protected:
    DatabaseManager *dbManager;
    int dataModelIndex_id;
    int dataModelIndex_name;
};

#endif // EFCOMBOBOX_H

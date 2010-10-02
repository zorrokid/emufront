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

#include "setupcombobox.h"
#include "../db/dbsetup.h"

SetupComboBox::SetupComboBox(DatabaseManager *dbMan, QWidget *parent)
    : EFComboBox(dbMan, parent)
{
    dataModelIndex_name = DbSetup::Setup_Name;
    dataModelIndex_id = DbSetup::Setup_Id;
    setModelColumn(dataModelIndex_name);

}


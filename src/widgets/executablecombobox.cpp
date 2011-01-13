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
#include "executablecombobox.h"
#include "dbexecutable.h"

ExecutableComboBox::ExecutableComboBox(DatabaseManager *dbMan, QWidget *parent)
    : EFComboBox(dbMan, parent)
{
    dataModelIndex_name = DbExecutable::Executable_Name;
    dataModelIndex_id = DbExecutable::Executable_Id;
    setModelColumn(dataModelIndex_name);
    currentPlatformId = -1;
    currentMediaTypeId = -1;
}

void ExecutableComboBox::updateToSetup(int platformId, int mediaTypeId)
{
    currentPlatformId = platformId;
    currentMediaTypeId = mediaTypeId;
    reset();
}

void ExecutableComboBox::reset()
{
    DbExecutable *dbExe = dynamic_cast<DbExecutable *>(dbManager);
    dbExe->filterByPlatformMediaType(currentPlatformId, currentMediaTypeId);
    EFComboBox::updateDataModel(false);
}

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

#include <QtGui>
#include "executablemaindialog.h"
#include "executableeditdialog.h"
#include "../db/dbexecutable.h"
#include "../dataobjects/executable.h"

ExecutableMainDialog::ExecutableMainDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    setWindowTitle(tr("Emulators"));
    dbManager = new DbExecutable(this);
    initDataTable();
    initEditDialog();
    objectList->hideColumn(DbExecutable::Executable_Id);
    objectList->hideColumn(DbExecutable::Executable_TypeId);
    objectList->hideColumn(DbExecutable::Executable_SetupId);
    connectSignals();
}

ExecutableMainDialog::~ExecutableMainDialog()
{
    deleteCurrentObject();
}

void ExecutableMainDialog::initEditDialog()
{
    nameDialog = new ExecutableEditDialog(
        this, dynamic_cast<Executable*>(dbObject));
}

void ExecutableMainDialog::deleteCurrentObject()
{
   delete dynamic_cast<Executable*>(dbObject);
   dbObject = 0;
}

EmuFrontObject* ExecutableMainDialog::createObject()
{
    Executable *ex = new Executable;
    return ex;
}

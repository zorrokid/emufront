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

#include <QtGui>
#include "executablemaindialog.h"
#include "executableeditdialog.h"
#include "dbexecutable.h"
#include "executable.h"

ExecutableMainDialog::ExecutableMainDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    setWindowTitle(tr("Emulators"));
    dbManager = new DbExecutable(this);
    initDataTable();
    initEditDialog();
    hiddenColumns << DbExecutable::Executable_Id;
    hiddenColumns << DbExecutable::Executable_TypeId;
    hiddenColumns << DbExecutable::Executable_SetupId;
    hideColumns();
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
    connectNameDialogSignals();
}

void ExecutableMainDialog::deleteCurrentObject()
{
    if (dbObject) {
        Executable *exe =  dynamic_cast<Executable*>(dbObject);
        if (exe) delete exe;
        else qDebug() << "Failed deleting Executable";
        dbObject = 0;
    }
}

void ExecutableMainDialog::cleanUp()
{
    deleteCurrentObject();
    if (nameDialog) {
        ExecutableEditDialog *pnd =
            dynamic_cast<ExecutableEditDialog *>(nameDialog);
        if (pnd) delete pnd;
        else qDebug() << "Failed to delete PlatformNameDialog";
        nameDialog = 0;
    }
}

EmuFrontObject* ExecutableMainDialog::createObject()
{
    Executable *ex = new Executable;
    return ex;
}

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
#include "../db/dbplatform.h"
#include "../dataobjects/platform.h"
#include "platformdialog.h"
#include "platformnamedialog.h"

PlatformDialog::PlatformDialog(QWidget *parent)
    : EmuFrontFileObjectDialog(parent)
{
    setWindowTitle(tr("Set emulated platforms"));
    dbManager = new DbPlatform(this);
    initDataTable();
    initEditDialog();
    setColumnsHidden();
    hideColumns();

    // do not move to parent class:
    connectSignals();
}

PlatformDialog::~PlatformDialog()
{
    qDebug() << "PlatformDialog destructor.";
    deleteCurrentObject();
}

void PlatformDialog::initEditDialog()
{
    nameDialog = new PlatformNameDialog(this, dynamic_cast<Platform*>(dbObject));
    connectNameDialogSignals();
}

EmuFrontObject* PlatformDialog::createObject()
{
    qDebug() << "PlatformDialog creating a Platform object.";
    return new Platform;
}

void PlatformDialog::deleteCurrentObject()
{
    if (dbObject) {
        qDebug() << "PlatformDialog deleting a Platform object.";
        Platform *plf = dynamic_cast<Platform*>(dbObject);
        if (plf) delete plf;
        else qDebug() << "Failed deleting Platform";
        dbObject = 0;
    }
}

void PlatformDialog::cleanUp()
{
    deleteCurrentObject();
    if (nameDialog) {
        PlatformNameDialog *pnd =
            dynamic_cast<PlatformNameDialog*>(nameDialog);
        if (pnd) delete pnd;
        else qDebug() << "Failed to delete PlatformNameDialog";
        nameDialog = 0;
    }
}

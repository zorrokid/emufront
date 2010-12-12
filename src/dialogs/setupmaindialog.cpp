/*
** EmuFront
** Copyright 2010 Mikko Keinänen
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
#include <QtGui>
#include "setupmaindialog.h"
#include "setupeditdialog.h"
#include "dbsetup.h"

SetupMainDialog::SetupMainDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    setWindowTitle(tr("Setups"));
    dbManager = new DbSetup(this);
    initDataTable();
    initEditDialog();
    hiddenColumns << DbSetup::Setup_Id;
    hiddenColumns << DbSetup::Setup_PlatformId;
    hiddenColumns << DbSetup::Setup_MediaTypeId;
    hideColumns();
    connectSignals();
}

SetupMainDialog::~SetupMainDialog()
{
    deleteCurrentObject();
}

void SetupMainDialog::initEditDialog()
{
    if (nameDialog)  {
        SetupEditDialog *sed =
            dynamic_cast<SetupEditDialog*>(nameDialog);
        if (sed) delete sed;
        else qDebug() << "Failed to delete SetupEditDialog!";
    }
    nameDialog = new SetupEditDialog(this, dynamic_cast<Setup*>(dbObject));
    connectNameDialogSignals();
}

void SetupMainDialog::deleteCurrentObject()
{
    if (dbObject) {
        Setup *sup = dynamic_cast<Setup*>(dbObject);
        if (sup)
            delete sup;
        else
            qDebug() << "Failed to delete Setup data objec data object.";
        dbObject = 0;
    }
}

EmuFrontObject* SetupMainDialog::createObject()
{
    return new Setup;
}

void SetupMainDialog::cleanUp()
{
    deleteCurrentObject();
    if (nameDialog) {
        SetupEditDialog *sed =
                dynamic_cast<SetupEditDialog*>(nameDialog);
        if (sed) delete sed;
        else qDebug() << "Failed to delete SetupEditDialog!";
        nameDialog = 0;
    }
}

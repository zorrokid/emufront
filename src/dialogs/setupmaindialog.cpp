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

#include <QtGui>
#include "setupmaindialog.h"
#include "../db/dbsetup.h"

SetupMainDialog::SetupMainDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    setWindowTitle(tr("Setups"));
    //nameDialog = new SetupEditor(this, dynamic_cast<Setup*>(dbObject));
    dbManager = new DbSetup(this);
    initDataTable();
    connectSignals();
}

SetupMainDialog::~SetupMainDialog()
{
    deleteCurrentObject();
}

void SetupMainDialog::deleteCurrentObject()
{
   delete dynamic_cast<Setup*>(dbObject);
   dbObject = 0;
}

EmuFrontObject* SetupMainDialog::createObject()
{
    return new Setup;
}

/*void SetupMainDialog::connectSignals()
{
}*/

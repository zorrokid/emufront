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
#include "../db/dbplatform.h"
#include "../dataobjects/platform.h"
#include "platformdialog.h"
#include "platformnamedialog.h"

PlatformDialog::PlatformDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    setWindowTitle(tr("Set emulated platforms"));
    dbManager = new DbPlatform(this);
    initDataTable();
    initEditDialog();
    objectList->hideColumn(DbPlatform::Platform_Id);
    
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
}

EmuFrontObject* PlatformDialog::createObject()
{
    qDebug() << "PlatformDialog creating a Platform object.";
    return new Platform;
}

void PlatformDialog::deleteCurrentObject()
{
    qDebug() << "PlatformDialog deleting a Platform object.";
    delete dynamic_cast<Platform*>(dbObject);
    dbObject = 0;
}

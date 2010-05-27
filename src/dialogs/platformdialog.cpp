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
#include <QAbstractItemView>
#include <QSqlTableModel>
#include <QTextStream>
#include "../db/dbplatform.h"
#include "../dataobjects/platform.h"
#include "platformdialog.h"
#include "platformnamedialog.h"

PlatformDialog::PlatformDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    setWindowTitle(tr("Set emulated platforms"));
    nameDialog = new PlatformNameDialog(this, dynamic_cast<Platform*>(dbObject));
    dbManager = new DbPlatform(this);
    initDataTable();
    
    // do not move to parent class:
    connectSignals();
}

PlatformDialog::~PlatformDialog()
{
    deleteCurrentObject();
}

void PlatformDialog::addObject()
{
    /*if (!nameDialog)
    {
        if (!dbObject) dbObject = new Platform;
        nameDialog = new PlatformNameDialog(this, dynamic_cast<Platform*>(dbObject));
    }*/

    deleteCurrentObject();
    dbObject = new Platform;
    nameDialog->setDataObject(dbObject);
    activateNameDialog();
}

void PlatformDialog::deleteCurrentObject()
{
    delete dynamic_cast<Platform*>(dbObject);
}

bool PlatformDialog::deleteItem()
{
    qDebug() << "PlatformDialog::deleteItem()";
    QModelIndex index = objectList->currentIndex();
    if (!index.isValid()) return false;

    qDebug() << "Index is valid";

    // TODO: when implementing data bindings to platform
    // we need to check if platform being removed has bindings
    // and a) ask user if this platform should be removed
    // b) remove all the data associated to this platform

    EmuFrontObject *ob = dynamic_cast<DbPlatform*>(dbManager)->getDataObjectFromModel(&index);
    if (!ob) return false;

    Platform *plf = dynamic_cast<Platform*>(ob);

    qDebug() << "Got platform" << plf->getName();

    int numBindings = dynamic_cast<DbPlatform*>(dbManager)->countDataObjectRefs(plf->getId());
    if (numBindings > 0 && !confirmDelete(plf->getName(), numBindings))
    {
            return false;
    }
    delete plf;
    bool delOk = (dynamic_cast<DbPlatform *>(dbManager))->deleteDataObjectFromModel(&index);
    if (!delOk)
    {
        qDebug() << "delete failed";
        return false;
    }
    updateList();
    objectList->setFocus();
    return false;
}

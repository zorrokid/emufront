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


QTextStream cout(stdout, QIODevice::WriteOnly);

PlatformDialog::PlatformDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    setWindowTitle(tr("Set emulated platforms"));
    //nameDialog = 0;
    nameDialog = new PlatformNameDialog(this, dynamic_cast<Platform*>(dbObject));
    dbManager = new DbPlatform(this);

    // let's create a table model for platforms
    
    objectList->setModel(dbManager->getDataModel());
    objectList->setSelectionMode(QAbstractItemView::SingleSelection);
    //objectList->setColumnHidden(DatabaseManager::Platform_Id, true);
    objectList->resizeColumnsToContents();

    // do not move to parent class:
    connectSignals();
}

PlatformDialog::~PlatformDialog()
{
    delete dynamic_cast<Platform*>(dbObject);
}

int PlatformDialog::deleteObject()
{
    return 0;
}

void PlatformDialog::addObject()
{
    cout << "PlaformDialog::addObject" << endl;
    /*if (!nameDialog)
    {
        if (!dbObject) dbObject = new Platform;
        cout << "PlaformDialog::addObject: creating nameDialog..." << endl;
        nameDialog = new PlatformNameDialog(this, dynamic_cast<Platform*>(dbObject));
    }*/

    delete dynamic_cast<Platform*>(dbObject);
    dbObject = new Platform;
    // we need to fetch a new id for this platform
    nameDialog->setDataObject(dbObject);
    activateNameDialog();
}

void PlatformDialog::editObject()
{
    qDebug() << "editObject called_";
    QModelIndex index = objectList->currentIndex();
    if (!index.isValid())
        return;
    qDebug() << "we have a valid index";
    delete dbObject;
    dbObject = (dynamic_cast<DbPlatform*>(dbManager))->getPlatformFromModel(&index);
    nameDialog->setDataObject(dbObject);
    activateNameDialog();
}

void PlatformDialog::updateData()
{
    qDebug() << "Update data";
    // update data model
    if (!dbObject) return;

    qDebug() << "dbObject is not 0";

    QMessageBox::information(this, "Test", "We have a " + dbObject->getName());

    qDebug() << "Data will be inserted/updated...";

    // if data object id > -1 we are updating the data otherwise we are inserting new data
    if (dbObject->getId() > -1) updateDb(dbObject);
    else insertDb(dbObject);

    // refresh...
    DbObjectDialog::updateData();
}

void PlatformDialog::updateDb(const EmuFrontObject *ob) const
{
    if (!ob) return;
    qDebug() << "Updating platform " << ob->getName();
    (dynamic_cast<DbPlatform*>(dbManager))->updatePlatformToModel(dynamic_cast<const Platform*>(ob));
}

void PlatformDialog::insertDb(const EmuFrontObject *ob) const
{
    (dynamic_cast<DbPlatform*>(dbManager))->insertPlatformToModel(dynamic_cast<const Platform*>(ob));
}

bool PlatformDialog::deleteItem()
{
    QModelIndex index = objectList->currentIndex();
    if (!index.isValid()) return false;
    (dynamic_cast<DbPlatform *>(dbManager))->deletePlatformFromModel(&index);
    updateList();
    objectList->setFocus();
    return false;
}


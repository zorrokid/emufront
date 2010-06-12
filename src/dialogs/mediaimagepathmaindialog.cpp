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

#include "../dataobjects/filepathobject.h"
#include "../dataobjects/emufrontfileobject.h"
#include "../db/dbfilepath.h"
#include "../utils/fileutil.h"
#include "mediaimagepathmaindialog.h"
#include "mediaimagepathdialog.h"

MediaImagePathMainDialog::MediaImagePathMainDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    setWindowTitle(tr("Set media image paths"));
    dbManager = new DbFilePath(this);
    initDataTable();

    scanButton = new QPushButton(tr("&Scan"));
    buttonBox->addButton(scanButton, QDialogButtonBox::ActionRole);

    initEditDialog();
    objectList->hideColumn(DbFilePath::FilePath_Id);
    objectList->hideColumn(DbFilePath::FilePath_SetupId);
    // do not move to parent class:
    connectSignals();
}

void MediaImagePathMainDialog::connectSignals()
{
    DbObjectDialog::connectSignals();
    connect(scanButton, SIGNAL(clicked()), this, SLOT(beginScanFilePath()));
}

void MediaImagePathMainDialog::initEditDialog()
{
    nameDialog = new MediaImagePathDialog(this, dynamic_cast<FilePathObject*>(dbObject));
}

void MediaImagePathMainDialog::beginScanFilePath()
{
    QModelIndex index = objectList->currentIndex();
    if (!index.isValid()) return;
    FileUtil fileUtil(this);
    EmuFrontObject *ob = dbManager->getDataObjectFromModel(&index);
    if (!ob) return;
    FilePathObject *fpo = dynamic_cast<FilePathObject*>(ob);
    try
    {
        QStringList l;
        l << "*.zip"; // TODO set filters in a global constant class

        QList<MediaImageContainer*> files = fileUtil.scanFilePath(fpo, l);

        // TODO
    }
    catch (EmuFrontException s)
    {
        errorMessage->showMessage( s.what() );
   }
}

EmuFrontObject* MediaImagePathMainDialog::createObject()
{
    return new FilePathObject;
}

MediaImagePathMainDialog::~MediaImagePathMainDialog()
{
    deleteCurrentObject();
}

void MediaImagePathMainDialog::deleteCurrentObject()
{
    delete dynamic_cast<FilePathObject*>(dbObject);
    dbObject = 0;
}

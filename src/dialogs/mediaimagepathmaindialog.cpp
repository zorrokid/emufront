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
#include "../db/dbfilepath.h"
#include "mediaimagepathmaindialog.h"
#include "mediaimagepathdialog.h"

MediaImagePathMainDialog::MediaImagePathMainDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    qDebug() << "MediaImagePathMainDialog";
    setWindowTitle(tr("Set media image paths"));
    qDebug() << "Creating MediaImagePathDialog";
    nameDialog = new MediaImagePathDialog(this, dynamic_cast<FilePathObject*>(dbObject));
    qDebug() << "Creating DbFilePath";
    dbManager = new DbFilePath(this);
    qDebug() << "Initializing data table";
    initDataTable();

    scanButton = new QPushButton(tr("&Scan"));
    buttonBox->addButton(scanButton, QDialogButtonBox::ActionRole);

    qDebug() << "Connecting signals";
    // do not move to parent class:
    connectSignals();
}

void MediaImagePathMainDialog::connectSignals()
{
    DbObjectDialog::connectSignals();
    connect(scanButton, SIGNAL(clicked()), this, SLOT(scanFilePath()));
}

void MediaImagePathMainDialog::scanFilePath()
{
    qDebug() << "Scan file path requested";
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

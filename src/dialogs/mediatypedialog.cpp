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
#include "../db/dbmediatype.h"
#include "../dataobjects/mediatype.h"
#include "mediatypedialog.h"
#include "mediatypenamedialog.h"

MediaTypeDialog::MediaTypeDialog(QWidget* parent)
    :DbObjectDialog(parent)
{
    setWindowTitle(tr("Set media types"));
    dbManager = new DbMediaType(this);
    initDataTable();
    initEditDialog();
    objectList->hideColumn(DbMediaType::EmuFrontFileObject_FileId);

    // do not move to parent class:
    connectSignals();

}

MediaTypeDialog::~MediaTypeDialog()
{
    deleteCurrentObject();
}

void MediaTypeDialog::initEditDialog()
{
    nameDialog = new MediaTypeNameDialog(this, dynamic_cast<MediaType*>(dbObject));
}

EmuFrontObject* MediaTypeDialog::createObject()
{
    return new MediaType;
}

void MediaTypeDialog::deleteCurrentObject()
{
    delete dynamic_cast<MediaType*>(dbObject);
    dbObject = 0;
}

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
    //nameDialog = 0;
    nameDialog = new MediaTypeNameDialog(this, dynamic_cast<MediaType*>(dbObject));
    dbManager = new DbMediaType(this);
    initDataTable();

    // do not move to parent class:
    connectSignals();

}

MediaTypeDialog::~MediaTypeDialog()
{
    deleteCurrentObject();
}

void MediaTypeDialog::addObject()
{
    deleteCurrentObject();
    dbObject = new MediaType;
    nameDialog->setDataObject(dbObject);
    activateNameDialog();
}

void MediaTypeDialog::deleteCurrentObject()
{
    delete dynamic_cast<MediaType*>(dbObject);
}

int MediaTypeDialog::deleteObject()
{
    return 0;
}

void MediaTypeDialog::updateDb(const EmuFrontObject *ob) const
{
    if (!ob) return;
    qDebug() << "Updating media type" << ob->getName();
    (dynamic_cast<DbMediaType*>(dbManager))->updateDataObjectToModel(ob);
}

void MediaTypeDialog::insertDb(const EmuFrontObject *ob) const
{
    (dynamic_cast<DbMediaType*>(dbManager))->insertDataObjectToModel(ob);
}

bool MediaTypeDialog::deleteItem()
{
    qDebug() << "MediaTypeDialog::deleteItem()";
    QModelIndex index = objectList->currentIndex();
    if (!index.isValid()) return false;

    qDebug() << "Index is valid";

    // TODO: when implementing data bindings to platform
    // we need to check if platform being removed has bindings
    // and a) ask user if this platform should be removed
    // b) remove all the data associated to this platform

    EmuFrontObject *ob = dynamic_cast<DbMediaType*>(dbManager)->getDataObjectFromModel(&index);
    if (!ob) return false;

    MediaType *plf = dynamic_cast<MediaType*>(ob);

    qDebug() << "Got platform" << plf->getName();

    int numBindings = dynamic_cast<DbMediaType*>(dbManager)->countDataObjectRefs(plf->getId());
    if (numBindings > 0 && !confirmDelete(plf->getName(), numBindings))
    {
        return false;
    }
    delete plf;
    bool delOk = (dynamic_cast<DbMediaType *>(dbManager))->deleteDataObjectFromModel(&index);
    if (!delOk)
    {
        qDebug() << "delete failed";
        return false;
    }
    updateList();
    objectList->setFocus();
    return false;
}

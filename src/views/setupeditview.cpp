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
#include "setupeditview.h"
#include "setupmodel.h"
#include "comboboxdelegate.h"
#include "stringlistdelegate.h"
#include "platformmodel.h"
#include "mediatypemodel.h"
#include <QSqlTableModel>

SetupEditView::SetupEditView(SetupModel *supModel, QWidget *parent) :
    EmuFrontEditView(parent)//, model(supModel)
{
    model = supModel;
    //model = new SetupModel(this);
    objectList->setModel(model);
    PlatformModel *plfModel = new PlatformModel(this);
    ComboBoxDelegate *platformDelegate = new ComboBoxDelegate(
            plfModel,
            PlatformModel::EmuFrontFileObject_Id,
            PlatformModel::EmuFrontFileObject_Name,
            this
        );
    objectList->setItemDelegateForColumn(SetupModel::Setup_PlatformId, platformDelegate);
    MediaTypeModel *mdtModel = new MediaTypeModel(this);
    ComboBoxDelegate *mediatypeDelegate = new ComboBoxDelegate(
            mdtModel,
            MediaTypeModel::EmuFrontFileObject_Id,
            MediaTypeModel::EmuFrontFileObject_Name,
            this
        );
    objectList->setItemDelegateForColumn(SetupModel::Setup_MediaTypeId, mediatypeDelegate);

    StringListDelegate *fileTypeDelegate = new StringListDelegate(SetupModel::FILE_TYPE_EXTENSION_SEPARATOR, this);
    objectList->setItemDelegateForColumn(SetupModel::Setup_FileTypeExtensions, fileTypeDelegate);
    postInit();
}

/*
** EmuFront
** Copyright 2010-2011 Mikko Keinänen
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

SetupEditView::SetupEditView(PlatformModel *plfModel, MediaTypeModel *mdtModel, SetupModel *supModel, QWidget *parent) :
    EmuFrontEditView(parent), plfModel(plfModel), mdtModel(mdtModel)
{
    setWindowTitle(tr("Setups"));
    model = supModel;
    objectList->setModel(model);
    ComboBoxDelegate *platformDelegate = new ComboBoxDelegate(
            plfModel,
            PlatformModel::EmuFrontFileObject_Id,
            PlatformModel::EmuFrontFileObject_Name,
            this
        );
    objectList->setItemDelegateForColumn(SetupModel::Setup_PlatformId, platformDelegate);
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
	objectList->resizeColumnsToContents();
	// TODO: the following would be nice to have set more dynamically:
	objectList->verticalHeader()->setDefaultSectionSize(StringListDelegate::HEIGHT);
	/*objectList->resizeRowsToContents();
	objectList->verticalHeader()->resizeSections(QHeaderView::Fixed);
	objectList->horizontalHeader()->resizeSections(QHeaderView::Fixed);*/
	//objectList->resizeColumnToContents(SetupModel::Setup_FileTypeExtensions);
}

void SetupEditView::setHiddenColumns()
{
	hiddenColumns << SetupModel::Setup_Id;
	hiddenColumns << SetupModel::Setup_Name; 
}



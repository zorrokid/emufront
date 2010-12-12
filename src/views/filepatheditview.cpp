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
#include "filepatheditview.h"
#include "filepathmodel.h"
#include "setupmodel.h"
#include "comboboxdelegate.h"
#include "filesystembrowsedelegate.h"
#include <QtGui>

FilePathEditView::FilePathEditView(QWidget *parent) :
    EmuFrontEditView(parent)
{
    model = new FilePathModel(this);
    objectList->setModel(model);
    SetupModel *stupMdl = new SetupModel(this);
    ComboBoxDelegate *setupDelegate = new ComboBoxDelegate(
        stupMdl,
        SetupModel::Setup_Id,
        SetupModel::Setup_Name,
        this
    );
    objectList->setItemDelegateForColumn(FilePathModel::FilePath_SetupId, setupDelegate);
    FileSystemBrowseDelegate *fsBrowseDelegate = new FileSystemBrowseDelegate(this);
    objectList->setItemDelegateForColumn(FilePathModel::FilePath_Name, fsBrowseDelegate);
    postInit();
}

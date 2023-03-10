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

#include "externalexecutableeditview.h"
#include "externalexecutablemodel.h"
#include "setupmodel.h"
#include "comboboxdelegate.h"
#include <QtGui>

ExternalExecutableEditView::ExternalExecutableEditView(SetupModel *supModel, ExternalExecutableModel *emuModel, QWidget *parent) :
    EmuFrontEditView(parent)
{
    setWindowTitle(tr("Set emulators"));
    model = emuModel; // new ExternalExecutableModel(this);
    objectList->setModel(model);
    ComboBoxDelegate *setupDelegate = new ComboBoxDelegate(
        supModel,
        SetupModel::Setup_Id,
        SetupModel::Setup_Name,
        this
    );
    objectList->setItemDelegateForColumn(ExternalExecutableModel::Executable_SetupId, setupDelegate);
    postInit();
}

void ExternalExecutableEditView::setHiddenColumns() 
{
	hiddenColumns << ExternalExecutableModel::Executable_Id;
	hiddenColumns << ExternalExecutableModel::Executable_SetupName;
	hiddenColumns << ExternalExecutableModel::Executable_TypeId;
}


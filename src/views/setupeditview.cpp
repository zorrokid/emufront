// EmuFront
// Copyright 2010 Mikko Keinänen
//
// This file is part of EmuFront.
//
//
// EmuFront is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation and appearing in the file gpl.txt included in the
// packaging of this file.
//
// EmuFront is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#include <QtGui>
#include "setupeditview.h"
#include "setupmodel.h"
#include "comboboxdelegate.h"
#include "platformmodel.h"
#include <QSqlTableModel>

SetupEditView::SetupEditView(QWidget *parent) :
    EmuFrontEditView(parent)
{
    model = new SetupModel(this);
    objectList->setModel(model);
    /*QSqlTableModel *platformModel = new QSqlTableModel(this);
    platformModel->setTable("platform");
    platformModel->removeColumn(Plat)*/
    PlatformModel *plfModel = new PlatformModel(this);
    ComboBoxDelegate *platformDelegate = new ComboBoxDelegate(SetupModel::Setup_PlatformId, plfModel, PlatformModel::EmuFrontFileObject_Id, PlatformModel::EmuFrontFileObject_Name, this);
    objectList->setItemDelegate(platformDelegate);
    postInit();
}

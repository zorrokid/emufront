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
#include "emufrontfileobjecteditview.h"
#include "emufrontfileobjectmodel.h"

EmuFrontFileObjectEditView::EmuFrontFileObjectEditView(QWidget *parent) :
    EmuFrontEditView(parent)
{
}

void EmuFrontFileObjectEditView::setHiddenColumns()
{
    hiddenColumns << EmuFrontFileObjectModel::EmuFrontFileObject_Id;
    hiddenColumns << EmuFrontFileObjectModel::EmuFrontFileObject_FileId;
    hiddenColumns << EmuFrontFileObjectModel::EmuFrontFileObject_FileType;
    hiddenColumns << EmuFrontFileObjectModel::EmuFrontFileObject_FileCheckSum;
	hiddenColumns << EmuFrontFileObjectModel::EmuFrontFileObject_FileName;
	hiddenColumns << EmuFrontFileObjectModel::EmuFrontFileObject_FileUpdateTime;
	hiddenColumns << EmuFrontFileObjectModel::EmuFrontFileObject_FileSize;
}

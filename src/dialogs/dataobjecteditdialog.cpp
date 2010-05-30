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

#include <QDialogButtonBox>
#include "dataobjecteditdialog.h"

DataObjectEditDialog::DataObjectEditDialog(QWidget *parent, EmuFrontObject *ob, Qt::Orientation orientation)
    : EmuFrontDialog(parent), efObject(ob), orientation(orientation)
{
    qDebug("DataObjectEditDialog creating buttonBox");
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Abort, orientation);
}

void DataObjectEditDialog::connectSignals()
{
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptChanges()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(rejectChanges()));
}

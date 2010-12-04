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
#include "platformnamedialog.h"
#include "databasemanager.h"

PlatformNameDialog::PlatformNameDialog(QWidget *parent, Platform *efObj)
    : NameDialog(parent, efObj)
{
    setWindowTitle(tr("Set platform name"));
}

PlatformNameDialog::~PlatformNameDialog()
{
    // no need to delete efObject here, the calling widget will take care of it
    // delete dynamic_cast<Platform*>(efObject);
}

void PlatformNameDialog::setDataObject(QString name)
{
    efObject->setName(name);
    (dynamic_cast<Platform*>(efObject))->setFile(0);
}

void PlatformNameDialog::setDataObject(EmuFrontObject *ob)
{
    nameEdit->hide();
    if (!ob) return;
    efObject = dynamic_cast<Platform*>(ob);
    nameEdit->setText(efObject->getName());
    nameEdit->setFocus(Qt::TabFocusReason);
    nameEdit->show();
}

/*void PlatformNameDialog::keyPressEvent(QKeyEvent *event)
{
    nameEdit->setText( nameEdit->text().append(event->text()) );
    QDialog::keyPressEvent(event);
}*/


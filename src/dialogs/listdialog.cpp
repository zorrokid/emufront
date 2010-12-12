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
#include "listdialog.h"
#include <QtGui>
#include "emufrontobject.h"

ListDialog::ListDialog(QWidget *parent)
    : QDialog(parent)
{
    listView = new QListWidget(this);
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Vertical);

}

void ListDialog::setData(QList<EmuFrontObject *> list)
{
    foreach(EmuFrontObject *efo, list) {
        listView->addItem(efo->getName());
    }
}

void ListDialog::layout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(listView);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}

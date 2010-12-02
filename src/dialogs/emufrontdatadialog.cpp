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
#include "emufrontdatadialog.h"

EmuFrontDataDialog::EmuFrontDataDialog(QWidget *parent) :
    EmuFrontDialog(parent)
{
    editButton = new QPushButton(tr("&Edit"));
    editButton->setEnabled(false);
    addButton = new QPushButton(tr("&Add"));
    deleteButton = new QPushButton(tr("&Delete"));
    deleteButton->setEnabled(false);
    objectList = new QTableView(this);
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Vertical);
    buttonBox->addButton(editButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    // this be called from the implementing classes:
    //connectSignals();
    layout();
}

void EmuFrontDataDialog::postInit()
{
    connectSignals();
    setHiddenColumns();
    hideColumns();
}

void EmuFrontDataDialog::layout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(objectList);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}

void EmuFrontDataDialog::hideColumns()
{
    foreach(int c, hiddenColumns)
        objectList->hideColumn(c);
}

void EmuFrontDataDialog::connectSignals()
{
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    /*connect(objectList, SIGNAL(clicked(const QModelIndex &)),
        this, SLOT(listObjectClicked(const QModelIndex &)));*/
    connect(editButton, SIGNAL(clicked()), this, SLOT(editButtonClicked()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteButtonClicked()));
}

void EmuFrontDataDialog::editButtonClicked()
{
    qDebug() << "Edit button clicked";
}

void EmuFrontDataDialog::deleteButtonClicked()
{
    qDebug() << "Delete button clicked";
}

void EmuFrontDataDialog::addButtonClicked()
{
    int row = objectList->currentIndex().isValid() ?
        objectList->currentIndex().row() : 0;
    model->insertRows(row, 1);
    QModelIndex ind = model->index(row, 1);
    if (!ind.isValid()){
        return;
    }
    objectList->setCurrentIndex(ind);
    objectList->edit(ind);
}

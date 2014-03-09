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
#include "emufronteditview.h"

EmuFrontEditView::EmuFrontEditView(QWidget *parent) :
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

void EmuFrontEditView::postInit()
{
    connectSignals();
    setHiddenColumns();
    hideColumns();
    objectList->resizeColumnsToContents();
    objectList->horizontalHeader()->setStretchLastSection(true);
}

void EmuFrontEditView::layout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(objectList);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}

void EmuFrontEditView::hideColumns()
{
    foreach(int c, hiddenColumns)
        objectList->hideColumn(c);
}

void EmuFrontEditView::connectSignals()
{
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(objectList, SIGNAL(clicked(const QModelIndex &)),
        this, SLOT(listObjectClicked(const QModelIndex &)));
	connect(model, SIGNAL(dataChanged()), this, SLOT(onDataChanged()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editButtonClicked()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteButtonClicked()));
}

void EmuFrontEditView::editButtonClicked()
{
    if (!objectList->currentIndex().isValid())
        return;
    setButtonsEnabled(false);
	objectList->edit(objectList->currentIndex());
}

void EmuFrontEditView::deleteButtonClicked()
{
    setButtonsEnabled(false);
    if (!objectList->currentIndex().isValid())
        return;
    QString msg = tr("Are you sure you want to delete the selected item? If other data objects are assigned to the selected item, they will be also removed!");
    if (!confirm(msg))
        return;
    int row = objectList->currentIndex().row();
    if ( !model->removeRows(row, 1) ) {
        errorMessage->showMessage(tr("Failed removing selected item."));
    }
}


void EmuFrontEditView::addButtonClicked()
{
    int row = objectList->currentIndex().isValid() ?
        objectList->currentIndex().row() : 0;
    try {
        model->insertRows(row, 1);
    } catch(EmuFrontException e) {
        errorMessage->showMessage(tr("Failed inserting rows: %1").arg(e.what()));
        return;
    }
    QModelIndex ind = model->index(row, 1);
    if (!ind.isValid()){
        return;
    }
    objectList->setCurrentIndex(ind);
    objectList->edit(ind);
}

void EmuFrontEditView::listObjectClicked(const QModelIndex &index)
{
    setButtonsEnabled(index.isValid());
}

void EmuFrontEditView::setButtonsEnabled(bool b)
{
    editButton->setEnabled(b);
    deleteButton->setEnabled(b);
}

void EmuFrontEditView::setHiddenColumns()
{
    // default implementation
}

bool EmuFrontEditView::confirm(QString &msg)
{
    int r = QMessageBox::warning(this, tr("Confirm"), msg, QMessageBox::Yes | QMessageBox::No);
    if ( r == QMessageBox::No )
        return false;
    return true;
}

void EmuFrontEditView::onDataChanged()
{
	qDebug() << "Data changed";
}

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

#include <QtGui>
#include <QSqlTableModel>
#include "dbobjectdialog.h"
#include "../db/databasemanager.h"

DbObjectDialog::DbObjectDialog(QWidget *parent)
    : EmuFrontDialog(parent)
{
    dbObject = 0;
    dbManager = 0;
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

DbObjectDialog::~DbObjectDialog()
{
    // no need to explicitically delete widgets within a parented layout
    // they are automatically parented and will be deleted
    // dbManager is also parented and will be implicitically deleted
    // this must be deleted in an implementing class
    //delete dbObject;
}

void DbObjectDialog::connectSignals()
{
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(close()));
    connect(objectList, SIGNAL(clicked(const QModelIndex &)),
        this, SLOT(listObjectClicked(const QModelIndex &)));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editButtonClicked()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteButtonClicked()));
    connect(nameDialog, SIGNAL(dataObjectUpdated()), this, SLOT(updateData()));
}

void DbObjectDialog::editObject()
{
    QModelIndex index = objectList->currentIndex();
    if (!index.isValid())
        return;
    deleteCurrentObject();
    dbObject = dbManager->getDataObjectFromModel(&index);
    nameDialog->setDataObject(dbObject);
    activateNameDialog();
}

void DbObjectDialog::updateDb(const EmuFrontObject *ob) const
{
    if (!ob) return;
    qDebug() << "Updating platform " << ob->getName();
    dbManager->updateDataObjectToModel(ob);
}

void DbObjectDialog::updateList() const
{
    if (!dbManager) return;
    dbManager->resetModel();
}

void DbObjectDialog::addButtonClicked()
{
    disableSelection();
    addObject();
}

void DbObjectDialog::editButtonClicked()
{
    disableSelection();
    editObject();
}

void DbObjectDialog::deleteButtonClicked()
{
    QItemSelectionModel *selModel = objectList->selectionModel();
    if (!selModel->hasSelection()) return;

    QAbstractItemModel *tblModel = objectList->model();
    QModelIndex index = selModel->currentIndex();
    QVariant vName = tblModel->data(index);
    QString name = vName.toString();
    disableSelection();

    QString msg =  tr("Do you want to delete") + name + "?";
    int yn = QMessageBox::question(this, "Confirm", msg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (yn == QMessageBox::Yes)
    {
        qDebug() << "Deleting item..." << name << ".";
        deleteItem();
    }
}

void DbObjectDialog::layout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(objectList);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}

void DbObjectDialog::listObjectClicked(const QModelIndex &index)
{
    const QModelIndex *x;
    x = &index;
    qDebug() << "Row " << x->row() << ", column " << x->column() << " clicked.";
    setButtonsEnabled(index.isValid());
    if(!index.isValid()) 
	return;
}

void DbObjectDialog::setButtonsEnabled(bool enabled)
{
    editButton->setEnabled(enabled);
    deleteButton->setEnabled(enabled);
}

void DbObjectDialog::disableSelection()
{
    setButtonsEnabled(false);
}

void DbObjectDialog::activateNameDialog()
{
    if (!nameDialog) return;
    nameDialog->show();
    nameDialog->raise();
    nameDialog->activateWindow();
}

void DbObjectDialog::initDataTable()
{
   objectList->setModel(dbManager->getDataModel());
   objectList->setSelectionMode(QAbstractItemView::SingleSelection);
   objectList->resizeColumnsToContents();
}
void DbObjectDialog::updateData()
{
    qDebug() << "Update data";
    // update data model
    if (!dbObject) return;

    qDebug() << "dbObject is not 0";

    qDebug() << "We have a " + dbObject->getName();

    qDebug() << "Data will be inserted/updated...";

    // if data object id > -1 we are updating the data otherwise we are inserting new data
    if (dbObject->getId() > -1) updateDb(dbObject);
    else insertDb(dbObject);

    // we don't need dbObject anymore
    deleteCurrentObject();
    dbObject = 0;
    updateList();
}

void DbObjectDialog::deleteCurrentObject()
{
    delete dbObject;
}

bool DbObjectDialog::confirmDelete(QString name, int numRefs)
{
    int r = QMessageBox::warning(this, tr("Confirm delete"),
                                 QString("Do you really want to delete %1 with %2 data bindings?")
                                 .arg(name).arg(numRefs),
                                 QMessageBox::Yes | QMessageBox::No);
    if ( r == QMessageBox::No )
        return false;
    return true;
}

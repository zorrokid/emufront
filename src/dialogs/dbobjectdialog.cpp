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
#include <QSqlTableModel>
#include "dbobjectdialog.h"
#include "databasemanager.h"

DbObjectDialog::DbObjectDialog(QWidget *parent)
    : EmuFrontDialog(parent)
{
    dbObject = 0;
    dbManager = 0;
    nameDialog = 0;
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
    // no need to explicitly delete widgets within a parented layout
    // they are automatically parented and will be deleted
    // dbManager is also parented and will be implicitly deleted
    // this must be deleted in an implementing class
    //delete dbObject;
    // hiddenColumn QList will be deleted implicitly, since it
    // implements implicit data sharing
}

void DbObjectDialog::connectSignals()
{
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(objectList, SIGNAL(clicked(const QModelIndex &)),
        this, SLOT(listObjectClicked(const QModelIndex &)));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editButtonClicked()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteButtonClicked()));
}

void DbObjectDialog::connectNameDialogSignals()
{
    connect(nameDialog, SIGNAL(dataObjectUpdated()), this, SLOT(updateData()));
    connect(nameDialog, SIGNAL(updateRejected()), this, SLOT(updateReject()));
    connect(nameDialog, SIGNAL(test()), this, SLOT(testSlot()));
    connect(nameDialog, SIGNAL(dialogClosed()), this, SLOT(enableUi()));
}
    
void DbObjectDialog::testSlot()
{
    qDebug() << "TEST SIGNAL RECEIVED!";
}

void DbObjectDialog::insertDb(const EmuFrontObject *ob) const
{
    if (!dbManager->insertDataObjectToModel(ob))
        errorMessage->showMessage(tr("Inserting data object %1 failed.").arg(ob->getName()));
}

void DbObjectDialog::createEditDialog()
{
    initEditDialog();
    // call this from implementing classes:
    //connectNameDialogSignals();
}

void DbObjectDialog::addObject()
{
    setUIEnabled(false);
    if (!nameDialog) {
       createEditDialog();
    }
    deleteCurrentObject();
    dbObject = createObject();
    nameDialog->setDataObject(dbObject);
    activateNameDialog();
}

void DbObjectDialog::editObject()
{
    QModelIndex index = objectList->currentIndex();
    if (!index.isValid())
        return;
    if (!nameDialog) {
        createEditDialog();
    }
    deleteCurrentObject();
    try {
        dbObject = dbManager->getDataObjectFromModel(&index); // throws EmuFrontException
    } catch (EmuFrontException &e) {
        errorMessage->showMessage(e.what());
        return;
    }
    activateNameDialog();
    nameDialog->setDataObject(dbObject);
}

bool DbObjectDialog::deleteItem()
{
    qDebug() << "deleteItem called";
    QModelIndex index = objectList->currentIndex();
    if (!index.isValid()) return false;
    try
    {
        EmuFrontObject *ob = dbManager->getDataObjectFromModel(&index); // throws EmuFrontException

        qDebug() << "Trying to delete " << ob->getName();

        if (!ob)
        {
            errorMessage->showMessage(tr("Couldn't find the selected data object from data model!"));
            return false;
        }

        int numBindings = dbManager->countDataObjectRefs(ob->getId());

        if (numBindings > 0 && !confirmDelete(ob->getName(), numBindings))
        { return false; }
        deleteCurrentObject();

        bool delOk = dbManager->deleteDataObject(ob->getId());
        if (!delOk)
        {
            errorMessage->showMessage(tr("Deleting data object %1 failed!").arg(ob->getName()));
            return false;
        }
        updateList();
        objectList->setFocus();
        setUIEnabled(true);
    }
    catch(EmuFrontException e)
    {
        errorMessage->showMessage(e.what());
    }
    return false;
}

void DbObjectDialog::updateDb(const EmuFrontObject *ob) const
{
    if (!ob) return;
    if ( !dbManager->updateDataObjectToModel(ob) )
    { errorMessage->showMessage("Database update failed!"); }
}

void DbObjectDialog::updateList() const
{
    if (!dbManager) return;
    dbManager->resetModel();
    objectList->resizeColumnsToContents();
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

void DbObjectDialog::enableUi()
{
    setUIEnabled(true);
}

void DbObjectDialog::setButtonsEnabled(bool enabled)
{
    addButton->setEnabled(enabled);
    editButton->setEnabled(enabled);
    deleteButton->setEnabled(enabled);
}

void DbObjectDialog::setUIEnabled(bool enabled)
{
    buttonBox->setEnabled(enabled);
    objectList->setEnabled(enabled);
}

void DbObjectDialog::disableSelection()
{
    setUIEnabled(false);
    //setButtonsEnabled(false);
}

void DbObjectDialog::activateNameDialog(bool updateData)
{
    if (!nameDialog) return;
    nameDialog->show();
    nameDialog->raise();
    if (updateData)
        nameDialog->updateData();
    nameDialog->activateWindow();
}

void DbObjectDialog::initDataTable()
{
   objectList->setModel(dbManager->getDataModel());
   objectList->setSelectionMode(QAbstractItemView::SingleSelection);
   objectList->resizeColumnsToContents();
}

void DbObjectDialog::updateReject()
{
    addButton->setEnabled(true);
    setUIEnabled(true);
    // we don't want to keep this in memory
    deleteCurrentObject();
}

void DbObjectDialog::updateData()
{
    // update data model
    if (!dbObject) return;

    // if data object id > -1 we are updating the data otherwise we are inserting new data
    if (dbObject->getId() > -1) updateDb(dbObject);
    else insertDb(dbObject);

    // we don't need dbObject anymore
    deleteCurrentObject();
    dbObject = 0;
    updateList();
    setUIEnabled(true);
}

/* Implementation specific delete must be used!
void DbObjectDialog::deleteCurrentObject()
{
    delete dbObject;
}*/

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

void DbObjectDialog::refreshDataModel()
{
    dbManager->resetModel();
}

void DbObjectDialog::hideColumns()
{
    foreach(int c, hiddenColumns)
        objectList->hideColumn(c);
}

/* Enables UI. Deletes nameDialog object and current data object */
void DbObjectDialog::closeEvent(QCloseEvent */*ev*/)
{
    qDebug() << "DbObjectDialog closing!";
    setUIEnabled(true);
    cleanUp();
}

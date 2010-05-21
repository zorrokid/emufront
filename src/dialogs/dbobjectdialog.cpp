#include <QtGui>
#include <QSqlTableModel>
#include "dbobjectdialog.h"
#include "../db/databasemanager.h"

DbObjectDialog::DbObjectDialog(QWidget *parent)
    : EmuFrontDialog(parent)
{
    dbObject = 0;
    dbManager = new DatabaseManager(this);
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
    // nameDialog will be created on request
    
    //connectSignals();
    layout();
} 

DbObjectDialog::~DbObjectDialog()
{
    delete dbObject;
}

void DbObjectDialog::connectSignals()
{
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(close()));
    connect(objectList, SIGNAL(clicked(const QModelIndex &)),
        this, SLOT(listObjectClicked(const QModelIndex &)));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editButtonClicked()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteButtonClicked()));
    //connect(nameDialog, SIGNAL(accepted()), this, SLOT(updateList()));
    connect(nameDialog, SIGNAL(dataObjectUpdated()), this, SLOT(updateData()));
}

void DbObjectDialog::updateList() const
{
    sqlTableModel->setFilter("");
    sqlTableModel->select();
        // fetch items from database (virtual function for this)
        // update the item list
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

void DbObjectDialog::updateData()
{
    qDebug() << "DbObjectDialog::updateData()";
    updateList();
}

void DbObjectDialog::activateNameDialog() const
{
    if (!nameDialog) return;
    nameDialog->show();
    nameDialog->raise();
    nameDialog->activateWindow();
}


#include <QtGui>
#include "dbobjectdialog.h"
#include "../db/databasemanager.h"

DbObjectDialog::DbObjectDialog(QWidget *parent)
    : EmuFrontDialog(parent)
{
    dbManager = new DatabaseManager(this);
    editButton = new QPushButton(tr("&Edit")); 
    addButton = new QPushButton(tr("&Add"));
    deleteButton = new QPushButton(tr("&Delete"));
    objectList = new QTableView(this);
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Vertical);
    buttonBox->addButton(editButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    // nameDialog will be created on request
    
    connectSignals();
    layout();
} 

void DbObjectDialog::connectSignals()
{
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(close()));
    connect(objectList, SIGNAL(clicked(const QModelIndex &)), 
		this, SLOT(listObjectClicked(const QModelIndex &)));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editButtonClicked()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteButtonClicked()));
    connect(nameDialog, SIGNAL(accepted()), this, SLOT(updateList()));
}

void DbObjectDialog::updateList() const
{
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
}

void DbObjectDialog::deleteButtonClicked()
{
    disableSelection();
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

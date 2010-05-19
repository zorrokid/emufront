#include <QtGui>
#include <QAbstractItemView>
#include <QSqlTableModel>
#include <QTextStream>

#include "../db/databasemanager.h"
#include "../dataobjects/platform.h"
#include "platformdialog.h"
#include "platformnamedialog.h"


QTextStream cout(stdout, QIODevice::WriteOnly);

PlatformDialog::PlatformDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    setWindowTitle(tr("Set emulated platforms"));
    //nameDialog = 0;
    nameDialog = new PlatformNameDialog(this, dynamic_cast<Platform*>(dbObject));

    // let's create a table model for platforms
    sqlTableModel = dbManager->getPlatforms();
    objectList->setModel(sqlTableModel);
    objectList->setSelectionMode(QAbstractItemView::SingleSelection);
    objectList->setColumnHidden(DatabaseManager::Platform_Id, true);
    objectList->resizeColumnsToContents();

    // do not move to parent class:
    connectSignals();
}

int PlatformDialog::deleteObject()
{
    return 0;
}

void PlatformDialog::addObject()
{
    cout << "PlaformDialog::addObject" << endl;
    /*if (!nameDialog)
    {
        if (!dbObject) dbObject = new Platform;
        cout << "PlaformDialog::addObject: creating nameDialog..." << endl;
        nameDialog = new PlatformNameDialog(this, dynamic_cast<Platform*>(dbObject));
    }*/
    if (!dbObject) dbObject = new Platform;
    nameDialog->setDataObject(dbObject);
    nameDialog->show();
    nameDialog->raise();
    nameDialog->activateWindow();
}

void PlatformDialog::editObject()
{
}

void PlatformDialog::updateData()
{
    // update data model
    if (!dbObject) return;


    QMessageBox::information(this, "Test", "We have a " + dbObject->getName());

    int row = 0;
    sqlTableModel->insertRows(row, 1);
    sqlTableModel->setData(sqlTableModel->index(row, 0), dbObject->getId());
    sqlTableModel->setData(sqlTableModel->index(row, 1), dbObject->getName());
    sqlTableModel->setData(sqlTableModel->index(row, 2),
                            (dynamic_cast<Platform*>(dbObject))->getFilename());
    sqlTableModel->submitAll();

    // refresh...
    DbObjectDialog::updateData();
}

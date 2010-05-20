#include <QtGui>
#include <QAbstractItemView>
#include <QSqlTableModel>
#include <QTextStream>
#include <QSqlRecord>
#include <QSqlQuery>

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
    //objectList->setColumnHidden(DatabaseManager::Platform_Id, true);
    objectList->resizeColumnsToContents();

    // do not move to parent class:
    connectSignals();
}

PlatformDialog::~PlatformDialog()
{
    delete dynamic_cast<Platform*>(dbObject);
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

    delete dynamic_cast<Platform*>(dbObject);
    dbObject = new Platform;
    // we need to fetch a new id for this platform
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
    //sqlTableModel->setData(sqlTableModel->index(row, 0), NULL);
    sqlTableModel->setData(sqlTableModel->index(row, 1), dbObject->getName());
    sqlTableModel->setData(sqlTableModel->index(row, 2),
                            (dynamic_cast<Platform*>(dbObject))->getFilename());
    sqlTableModel->submitAll();

    // refresh...
    DbObjectDialog::updateData();
}

bool PlatformDialog::deleteItem()
{
    QModelIndex index = objectList->currentIndex();
    if (!index.isValid()) return false;

    QSqlDatabase::database().transaction();
    QSqlRecord record = sqlTableModel->record(index.row());
    int id = record.value(DatabaseManager::Platform_Id).toInt();
    int numEntries = 0;
    QSqlQuery query(QString("SELECT COUNT(*) FROM imagecontainer WHERE platformid = %1").arg(id));
    if (query.next())
        numEntries = query.value(0).toInt();
    if (numEntries > 0)
    {
        int r = QMessageBox::warning(this, tr("Delete platform"),
                                     QString("Do you want to delete platform %1 and all the related data?")
                                     .arg(record.value(DatabaseManager::Platform_Name).toString()), QMessageBox::Yes | QMessageBox::No);
        if ( r == QMessageBox::No )
        {
            QSqlDatabase::database().rollback();
            return false;
        }
        query.exec(QString("DELETE FROM imagecontainer WHERE platformid = %1").arg(id));
    }
    sqlTableModel->removeRow(index.row());
    sqlTableModel->submitAll();
    QSqlDatabase::database().commit();
    updateList();
    objectList->setFocus();
    return false;
}

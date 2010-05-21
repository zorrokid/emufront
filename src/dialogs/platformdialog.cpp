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

const QString PlatformDialog::DB_TABLE_NAME_PLATFORM = QString("platform");

PlatformDialog::PlatformDialog(QWidget *parent)
    : DbObjectDialog(parent)
{
    setWindowTitle(tr("Set emulated platforms"));
    //nameDialog = 0;
    nameDialog = new PlatformNameDialog(this, dynamic_cast<Platform*>(dbObject));

    // let's create a table model for platforms
    sqlTableModel = getDataObjects();
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
    activateNameDialog();
}

void PlatformDialog::editObject()
{
    qDebug() << "editObject called_";
    QModelIndex index = objectList->currentIndex();
    if (!index.isValid())
        return;
    qDebug() << "we have a valid index";
    QSqlRecord record = sqlTableModel->record(index.row());
    int id = record.value(Platform_Id).toInt();
    QString name = record.value(Platform_Name).toString();
    QString fileName = record.value(Platform_Filename).toString();
    qDebug() << "Name " << name << " id " << id;
    //EmuFrontObject *plf = new Platform(id, name, fileName);
    delete dbObject;
    dbObject = new Platform(id, name, fileName);
    nameDialog->setDataObject(dbObject);
    activateNameDialog();
}

void PlatformDialog::updateData()
{
    qDebug() << "Update data";
    // update data model
    if (!dbObject) return;

    qDebug() << "dbObject is not 0";

    QMessageBox::information(this, "Test", "We have a " + dbObject->getName());

    qDebug() << "Data will be inserted/updated...";

    // if data object id > -1 we are updating the data otherwise we are inserting new data
    if (dbObject->getId() > -1) updateDb(dbObject);
    else insertDb(dbObject);

    // refresh...
    DbObjectDialog::updateData();
}

void PlatformDialog::updateDb(const EmuFrontObject *ob) const
{
    if (!ob) return;
    qDebug() << "Updating platform " << ob->getName();
    sqlTableModel->setFilter(QString("id = %1").arg(ob->getId()));
    sqlTableModel->select();
    if (sqlTableModel->rowCount() == 1)
    {
        QSqlRecord record = sqlTableModel->record(0);
        record.setValue("name", ob->getName());
        record.setValue("filename", (dynamic_cast<const Platform *>(ob))->getFilename());
        sqlTableModel->setRecord(0, record);
        sqlTableModel->submitAll();
    }
}

void PlatformDialog::insertDb(const EmuFrontObject *ob) const
{
    qDebug() << "Inserting platform " << ob->getName();
    int row = 0;
    sqlTableModel->insertRows(row, 1);
    //sqlTableModel->setData(sqlTableModel->index(row, 0), NULL);
    sqlTableModel->setData(sqlTableModel->index(row, 1), ob->getName());
    sqlTableModel->setData(sqlTableModel->index(row, 2),
                            (dynamic_cast<const Platform*>(ob))->getFilename());
    sqlTableModel->submitAll();
}

bool PlatformDialog::deleteItem()
{
    QModelIndex index = objectList->currentIndex();
    if (!index.isValid()) return false;

    QSqlDatabase::database().transaction();
    QSqlRecord record = sqlTableModel->record(index.row());
    int id = record.value(Platform_Id).toInt();
    int numEntries = 0;
    QSqlQuery query(QString("SELECT COUNT(*) FROM imagecontainer WHERE platformid = %1").arg(id));
    if (query.next())
        numEntries = query.value(0).toInt();
    if (numEntries > 0)
    {
        int r = QMessageBox::warning(this, tr("Delete platform"),
                                     QString("Do you want to delete platform %1 and all the related data?")
                                     .arg(record.value(Platform_Name).toString()), QMessageBox::Yes | QMessageBox::No);
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

QSqlTableModel* PlatformDialog::getDataObjects()
{
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable(DB_TABLE_NAME_PLATFORM);
    model->setSort(Platform_Name, Qt::AscendingOrder);
    model->setHeaderData(Platform_Name, Qt::Horizontal, tr("Name"));
    model->select();
    return model;
}


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
// EmuFront is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDateTime>
#include "dbfile.h"

DbFile::DbFile(QObject *parent) : DbTableModelManager(parent)
{ }

EmuFrontObject* DbFile::recordToDataObject(const QSqlRecord *record)
{
    int id = record->value(File_Id).toInt();
    QString name = record->value(File_Name).toString();
    QString checksum = record->value(File_CheckSum).toString();
    int size = record->value(File_FileSize).toInt();
    int type = record->value(File_FileType).toInt();
    return new EmuFrontFile(id, name, checksum, size, type);
}

bool DbFile::updateDataObjectToModel(const EmuFrontObject *ob)
{
    const EmuFrontFile *plf = dynamic_cast<const EmuFrontFile*>(ob);
    bool ret = false;
    QSqlTableModel *tmodel = dynamic_cast<QSqlTableModel*>(sqlTableModel);
    tmodel->setFilter(QString("id = %1").arg(plf->getId()));
    tmodel->select();
    if (tmodel->rowCount() == 1)
    {
        QSqlRecord record = tmodel->record(0);
        record.setValue("name", plf->getName());
        record.setValue("type", plf->getType());
        record.setValue("checksum", plf->getCheckSum());
        record.setValue("size", plf->getSize());
        record.setValue("updatetime", QDateTime::currentDateTime().toTime_t());
        tmodel->setRecord(0, record);
        ret = tmodel->submitAll();
    }
    resetModel();
    return ret;
}

bool DbFile::insertDataObjectToModel(const EmuFrontObject *ob)
{
    const EmuFrontFile *plf = dynamic_cast<const EmuFrontFile*>(ob);
    int row = 0;
    QSqlTableModel *tmodel = dynamic_cast<QSqlTableModel*>(sqlTableModel);
    tmodel->insertRows(row, 1);
    // the null value for index will be set implicitily
    // when we don't assign any value to cell 0 in the sql table model
    //sqlTableModel->setData(sqlTableModel->index(row, 0), NULL);
    tmodel->setData(sqlTableModel->index(row, File_Name), plf->getName());
    tmodel->setData(sqlTableModel->index(row, File_FileType), plf->getType());
    tmodel->setData(sqlTableModel->index(row, File_CheckSum), plf->getCheckSum());
    tmodel->setData(sqlTableModel->index(row, File_FileSize), plf->getSize());
    tmodel->setData(sqlTableModel->index(row, File_UpdateTime),
        QDateTime::currentDateTime().toTime_t());
    return tmodel->submitAll();
}

int DbFile::countDataObjectRefs(int id) const
{
    return 0; // TODO
    // return countRows("imagecontainer", "platformid", id);
}

// WARNING: this will delete also all the databindings to selected platform
// the delete must be confirmed in the UI
bool DbFile::deleteDataObjectFromModel(QModelIndex *index)
{
    /*QSqlDatabase::database().transaction();*/
    QSqlTableModel *tmodel = dynamic_cast<QSqlTableModel*>(sqlTableModel);
    /*QSqlRecord record = tmodel->record(index->row());
    int id = record.value(File_Id).toInt();
    int count = countDataObjectRefs(id);
    if (count > 0)
    {
        QSqlQuery query;
        if (!query.exec(QString("DELETE FROM imagecontainer WHERE platformid = %1").arg(id)))
        {
            qDebug() << "Deleting data bindings failed!";
            QSqlDatabase::database().rollback();
            return false;
        }
    }*/
    tmodel->removeRow(index->row());
    tmodel->submitAll();
    return QSqlDatabase::database().commit();
}

QSqlQueryModel* DbFile::getData()
{
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable(DB_TABLE_NAME_FILE);
    model->setSort(File_Name, Qt::AscendingOrder);
    model->setHeaderData(File_Name, Qt::Horizontal, tr("Name"));
    model->setHeaderData(File_FileType, Qt::Horizontal, tr("Type"));
    model->setHeaderData(File_CheckSum, Qt::Horizontal, tr("Checksum"));
    model->setHeaderData(File_FileSize, Qt::Horizontal, tr("Size"));
    model->setHeaderData(File_UpdateTime, Qt::Horizontal, tr("Updated"));
    model->select();
    return model;
}

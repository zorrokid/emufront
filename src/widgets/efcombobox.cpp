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

#include "efcombobox.h"
#include "../exceptions/emufrontexception.h"
#include "../dataobjects/emufrontobject.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>

EFComboBox::EFComboBox(DatabaseManager *dbMan, QWidget *parent)
    : QComboBox(parent), dbManager(dbMan)
{
    if (!dbManager)
        throw new EmuFrontException("Database manager is not available!");
    updateDataModel();
}

EFComboBox::~EFComboBox()
{
}

void EFComboBox::updateDataModel()
{
    QSqlQueryModel *model = dbManager->getDataModel();
    if (model)
        setModel(model);
}

EmuFrontObject* EFComboBox::getSelected() const
{
    EmuFrontObject *efo = 0;
    int index = currentIndex();
    qDebug() << "Selected index " << index << ".";
    if (index < 0)
        return 0;
    QSqlQueryModel *qmodel
        = dynamic_cast<QSqlQueryModel*>(model());
    if (!qmodel) {
        qDebug() << "No data model available!";
        return 0;
    }
    QSqlRecord rec = qmodel->record(index);
    if (rec.isEmpty()) {
        qDebug() << "No record available!";
        return efo;
    }
    qDebug() << "Fetching a data object with id "
        << rec.value(dataModelIndex_id).toInt();
    EmuFrontObject *o
        = dbManager->getDataObject(rec.value(dataModelIndex_id).toInt());
    return o;
}

void EFComboBox::setSelected(const EmuFrontObject *efo)
{
    if (!efo)
        return;
    QSqlQueryModel *qmodel
        = dynamic_cast<QSqlQueryModel*>(model());
    for (int i = 0; i < qmodel->rowCount(); i++){
        QSqlRecord rec = qmodel->record(i);
        if (rec.value(dataModelIndex_id) == efo->getId()){
            setCurrentIndex(i);
            break;
        }
    }

}

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
#include "efcombobox.h"
#include "emufrontexception.h"
#include "emufrontobject.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>
#include <QTime>
#include <QAbstractItemView>

EFComboBox::EFComboBox(DatabaseManager *dbMan, QWidget *parent)
    : QComboBox(parent), dbManager(dbMan)
{
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
    updateDataModel();
}

EFComboBox::~EFComboBox()
{
}

void EFComboBox::updateDataModel(bool reset)
{
    QSqlQueryModel *model = dbManager->getDataModel(reset);
    if (model)
        setModel(model);
}

/*
    Returns a pointer to EmuFrontObject which should be deleted
    later by calling code!

    Throws EmuFrontException
*/
EmuFrontObject* EFComboBox::getSelected()
{
    EmuFrontObject *efo = 0;
    int index = currentIndex();
    if (index < 0)
        return 0;
    QSqlQueryModel *qmodel
        = dynamic_cast<QSqlQueryModel*>(model());
    if (!qmodel) {
        throw EmuFrontException(tr("No data model available!"));
    }
    QSqlRecord rec = qmodel->record(index);
    if (rec.isEmpty()) {
        throw EmuFrontException(tr("No data available for selected item!"));
    }
    int id = rec.value(dataModelIndex_id).toInt();
    EmuFrontObject *o = dbManager->getDataObject(id); /* Throws EmuFrontException */
    reset();
    setCurrentIndex(index);
    if (!o) throw EmuFrontException(tr("Failed creating selected data object!"));
    return o;
}

void EFComboBox::reset()
{
    dbManager->resetModel();
}

void EFComboBox::setSelected(const EmuFrontObject *efo)
{
    if (!efo)
        return;
    qDebug() << "EFCombobox selecting " << efo->getName()
        << " [" << efo->getId() << "].";
    QSqlQueryModel *qmodel
        = dynamic_cast<QSqlQueryModel*>(model());
    QModelIndex idStart = qmodel->index(0, dataModelIndex_id);
    int targetId = efo->getId();

    QModelIndexList indLst = qmodel->match(idStart,Qt::DisplayRole, targetId, 1);
    if (indLst.count() >= 1) {
        QModelIndex ind = indLst.first();
        view()->setCurrentIndex(ind);
        setCurrentIndex(ind.row());
    }
}

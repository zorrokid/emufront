// EmuFront
// Copyright 2010 Mikko Keinänen
//
// This file is part of EmuFront.
//
//
// EmuFront is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation and appearing in the file gpl.txt included in the
// packaging of this file.
//
// EmuFront is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#include <QSqlTableModel>
#include "dbtablemodelmanager.h"

DbTableModelManager::DbTableModelManager(QObject *parent)
    : DatabaseManager(parent)
{
}

void DbTableModelManager::filterById(int id)
{
    return filterDataObjects(QString("id = %1").arg(id));
}

void DbTableModelManager::filterDataObjects(QString filter)
{
    if (!sqlTableModel) sqlTableModel = getDataModel(); // throw EmuFrontException("Data model not available!");
    QSqlTableModel *tmodel = dynamic_cast<QSqlTableModel*>(sqlTableModel);
    tmodel->setFilter(filter);
    tmodel->select();
}

void DbTableModelManager::clearFilters()
{
    QSqlTableModel *tmodel = dynamic_cast<QSqlTableModel*>(sqlTableModel);
    tmodel->setFilter("");
    tmodel->select();
}

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
// Foobar is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#include <QSqlTableModel>
#include "dbtablemodelmanager.h"

DbTableModelManager::DbTableModelManager(QObject *parent)
    : DatabaseManager(parent)
{
}

void DbTableModelManager::filterById(int id)
{
    if (!sqlTableModel) sqlTableModel = getDataModel(); // throw EmuFrontException("Data model not available!");
    QSqlTableModel *tmodel = dynamic_cast<QSqlTableModel*>(sqlTableModel);
    tmodel->setFilter(QString("id = %1").arg(id));
    tmodel->select();
}

void DbTableModelManager::clearFilters()
{
    QSqlTableModel *tmodel = dynamic_cast<QSqlTableModel*>(sqlTableModel);
    tmodel->setFilter("");
    tmodel->select();
}

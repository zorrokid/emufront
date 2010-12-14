/*
** EmuFront
** Copyright 2010 Mikko Keinänen
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
*/#ifndef EMUFRONTQUERYMODEL_H
#define EMUFRONTQUERYMODEL_H

#include <QSqlQueryModel>

class EmuFrontObject;
class Platform;
class MediaType;

class EmuFrontQueryModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    EmuFrontQueryModel(QObject *parent = 0);
    EmuFrontObject* getDataObject(int id);
    EmuFrontObject* getDataObject(const QModelIndex &index);
    EmuFrontObject* getObject(int row);
    static int getCurrentTimeStamp();

protected:
    QString tableName;
    QList<int> editableColumns;
    void filterDataObjects(QList<QString> filter);
    QString constructWhereByFilters(QList<QString> filters);
    EmuFrontObject* getDataObject(QString filter);
    virtual void filterById(int id);
    void clearFilters();
    virtual EmuFrontObject* recordToDataObject(const QSqlRecord* ) = 0;
    virtual QString constructFilterById(int id) const = 0;
    virtual QString constructSelect(QString whereClause = "") const = 0;

private:
    EmuFrontObject* getFilteredDataObject();
};

#endif // EMUFRONTQUERYMODEL_H

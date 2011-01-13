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

#ifndef FILEPATHMODEL_H
#define FILEPATHMODEL_H

#include "emufrontquerymodel.h"
class FilePathObject;

class FilePathModel : public EmuFrontQueryModel
{
    Q_OBJECT
public:
    FilePathModel(QObject *parent = 0);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual bool insertRows(int row, int count, const QModelIndex &parent);
    virtual bool removeRows(int row, int count, const QModelIndex &parent);
    enum { FilePath_Id = 0,
           FilePath_Name,
           FilePath_LastScanned,
           FilePath_SetupId,
           FilePath_SetupName,
           FilePath_FileTypeId
       };
    FilePathObject* getFilePathObject(const QModelIndex &index);
    virtual bool setScanned(int id);

signals:
	void dataChanged();


protected:
    virtual void refresh();
    virtual QString constructSelect(QString where = "") const;
    virtual bool setFilePath(int id, QString filePath);
    virtual bool setSetup(int isd, int setupId);

    // Implemented for EmuFrontQueryModel:
    virtual EmuFrontObject* recordToDataObject(const QSqlRecord* );
    virtual QString constructFilterById(int id) const;
};

#endif // FILEPATHMODEL_H

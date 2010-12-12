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
*/#ifndef SETUPMODEL_H
#define SETUPMODEL_H

#include "emufrontquerymodel.h"

class SetupModel : public EmuFrontQueryModel
{
    Q_OBJECT
public:
    SetupModel(QObject *parent = 0);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual bool insertRows(int row, int count, const QModelIndex &parent);
    virtual bool removeRows(int row, int count, const QModelIndex &parent);
    enum { Setup_Id = 0,
           Setup_PlatformId,
           Setup_MediaTypeId,
           Setup_FileTypeExtensions,
           Setup_Name };
    static const QString FILE_TYPE_EXTENSION_SEPARATOR;

protected:
    virtual void refresh();
    virtual QString constructSelect(QString where = "") const;
    virtual bool setPlatform(int id, int platformId);
    virtual bool setMediaType(int id, int platformId);
    virtual bool setSupportedExtensions(int id, QString);
};

#endif // SETUPMODEL_H

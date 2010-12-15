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
*/

#ifndef MEDIAIMAGECONTAINERMODEL_H
#define MEDIAIMAGECONTAINERMODEL_H

#include "emufrontquerymodel.h"

class MediaImageContainerModel : public EmuFrontQueryModel
{
    Q_OBJECT
public:
    MediaImageContainerModel(QObject *parent = 0);
    void filterBySetup(int setupId);
    enum {
        MIC_FileId,
        MIC_FileName,
        MIC_FileCheckSum,
        MIC_FileSize,
        MIC_FilePathId,
        MIC_FilePathName,
        MIC_SetupId,
        MIC_PlatformId,
        MIC_PlatformName,
        MIC_MediaTypeId,
        MIC_MediaTypeName
    };

protected:
    virtual void refresh();
    virtual QString constructSelect(QString where = "") const;
    // Implemented for EmuFrontQueryModel:
    virtual EmuFrontObject* recordToDataObject(const QSqlRecord* );
    virtual QString constructFilterById(int id) const;
};

#endif // MEDIAIMAGECONTAINERMODEL_H

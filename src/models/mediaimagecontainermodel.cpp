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

#include "mediaimagecontainer.h"
#include "mediaimagecontainermodel.h"
#include "filepathmodel.h"
#include "mediaimagemodel.h"
#include <QtSql>

MediaImageContainerModel::MediaImageContainerModel(QObject *parent) :
    EmuFrontQueryModel(parent)
{
}

QString MediaImageContainerModel::constructSelect(QString where) const
{
    // TODO, for a usual search need a "light" version of this select
    // and MediaImageContainer (only id, name)
    return QString(
        "SELECT file.id, file.name, file.checksum, file.size, "
        "        filepath.id, filepath.name, "
        "        setup.id, "
        "        platform.id, platform.name, "
        "        mediatype.id, mediatype.name "
        "FROM file "
        "INNER JOIN mediaimagecontainer_filepath ON mediaimagecontainer_filepath.fileid = file.id "
        "INNER JOIN filepath ON mediaimagecontainer_filepath.filepathid = filepath.id "
        "INNER JOIN setup ON filepath.setupid = setup.id "
        "INNER JOIN platform ON setup.platformid = platform.id "
        "INNER JOIN mediatype ON setup.mediatypeid = mediatype.id "
        "%1 "
        "ORDER BY file.name"
    ).arg(where);
}

QString MediaImageContainerModel::constructFilterById(int id) const
{
    return QString("file.id = %1").arg(id);
}

void MediaImageContainerModel::refresh()
{
    //setQuery(constructSelect());
    setHeaderData(MIC_FileId, Qt::Horizontal, tr("File id"));
    setHeaderData(MIC_FileName, Qt::Horizontal, tr("File Name"));
    setHeaderData(MIC_FileCheckSum, Qt::Horizontal, tr("File checksum"));
    setHeaderData(MIC_FileSize, Qt::Horizontal, tr("File Size"));
    setHeaderData(MIC_FilePathId, Qt::Horizontal, tr("File path id"));
    setHeaderData(MIC_FilePathName, Qt::Horizontal, tr("File path name"));
    setHeaderData(MIC_SetupId, Qt::Horizontal, tr("Setup id"));
    setHeaderData(MIC_PlatformId, Qt::Horizontal, tr("Platform id"));
    setHeaderData(MIC_PlatformName, Qt::Horizontal, tr("Platform name"));
    setHeaderData(MIC_MediaTypeId, Qt::Horizontal, tr("Media type id"));
    setHeaderData(MIC_MediaTypeName, Qt::Horizontal, tr("Media type name"));
}

EmuFrontObject* MediaImageContainerModel::recordToDataObject(const QSqlRecord *rec)
{
    // TODO: checks!
    MediaImageContainer *mic = 0;
    if (!rec) return mic;
    int id = rec->value(MIC_FileId).toInt();
    QString name = rec->value(MIC_FileName).toString();
    QString checksum = rec->value(MIC_FileCheckSum).toString();
    int size = rec->value(MIC_FileSize).toInt();
    int fpId = rec->value(MIC_FilePathId).toInt();
    FilePathModel fpModel;
    EmuFrontObject *efo = fpModel.getDataObject(fpId);
    FilePathObject *fpo = dynamic_cast<FilePathObject*>(efo);
    if (!fpo) return 0;
    //int supId = rec->value(MIC_SetupId).toInt();
    //Setup *sup = dbSetup->getDataObject(supId)
    MediaImageModel miModel;
    QMap<QString, EmuFrontObject*> images = miModel.getMediaImages(id);
    mic = new MediaImageContainer(
       id, name, checksum, size, images, fpo
    );
    return mic;
}

void MediaImageContainerModel::filterBySetup(int setupId)
{
    QList<QString> filters;
    filters.append(QString("setup.id = %1").arg(setupId));
    filterDataObjects(filters);
}

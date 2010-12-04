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

#include "setupmodel.h"

SetupModel::SetupModel(QObject *parent) :
    EmuFrontQueryModel(parent)
{
    refresh();
}

void SetupModel::refresh()
{
    setQuery(constructSelect());
    setHeaderData(Setup_Id, Qt::Horizontal, tr("Id"));
    setHeaderData(Setup_PlatformId, Qt::Horizontal, tr("Platform id"));
    setHeaderData(Setup_MediaTypeId, Qt::Horizontal, tr("Media type id"));
    setHeaderData(Setup_FileTypeExtensions, Qt::Horizontal, tr("File types"));
    setHeaderData(Setup_Name, Qt::Horizontal, tr("Name"));
}

QString SetupModel::constructSelect(QString where) const
{
    return QString(
        "SELECT setup.id AS SetupId, "
        "setup.platformid AS PlatformId, "
        "setup.mediatypeid AS MediaTypeId, "
        "setup.filetypeextensions AS SupportedFileTypeExtensions, "
        "platform.name || ' ' || mediatype.name AS SetupName "
        "FROM setup "
        "INNER JOIN platform ON setup.platformid=platform.id "
        "INNER JOIN mediatype ON setup.mediatypeid=mediatype.id %1 "
        "ORDER BY SetupName"
        ).arg(where);
}

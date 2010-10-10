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

#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <QObject>
#include "../dataobjects/emufrontfileobject.h"
#include "../dataobjects/filepathobject.h"

#define READ_BUFFER (256*1024)

class QStringList;
class MediaImageContainer;
class MediaImage;

class FileUtil : QObject
{
public:
    FileUtil(QObject *parent);
    ~FileUtil();
    QList<MediaImageContainer*> scanFilePath(FilePathObject *fpo, const QStringList filters);
private:
    char *buf;
    quint32 readCrc32(QString filePath);
    QList<MediaImage*>  listContents(const QString filePath, const FilePathObject *fp);
    bool isSupportedFile(const QString filename, const QStringList supportedFileExtensions);
};

#endif // FILEUTIL_H

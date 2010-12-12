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
*/#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <QObject>
#include "emufrontfileobject.h"
#include "filepathobject.h"

#define READ_BUFFER (256*1024)

class QStringList;
class MediaImageContainer;
class MediaImage;
class DbMediaImageContainer;
class UnzipHelper;
class QProgressDialog;

class FileUtil : public QObject
{
    Q_OBJECT
public:
    FileUtil(QObject *parent);
    ~FileUtil();
    int scanFilePath(FilePathObject *fpo, const QStringList filters, DbMediaImageContainer *mic, QProgressDialog *);
signals:
    void dbUpdateInProgress();
    void dbUpdateFinished();

private:
    char *buf;
    quint32 readCrc32(QString filePath);
    bool isSupportedFile(const QString filename, const QStringList supportedFileExtensions);
    static const int MIC_BUFFER_SIZE = 50;
    UnzipHelper *unzipHelper;
    void showDbUpdating(QProgressDialog*);
    void hideDbUpdating(QProgressDialog*);

};

#endif // FILEUTIL_H

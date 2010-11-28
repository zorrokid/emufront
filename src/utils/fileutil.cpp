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

#include <QDir>
#include <QDebug>
#include <QProcess>
#include <QProgressDialog>
#include "fileutil.h"
#include "zlib.h" /* crc32 */
#include "../exceptions/emufrontexception.h"
#include "../dataobjects/setup.h"
#include "../dataobjects/mediaimage.h"
#include "../dataobjects/mediaimagecontainer.h"
#include "../dataobjects/mediatype.h"
#include "../dataobjects/platform.h"
#include "../db/dbmediaimagecontainer.h"
#include "unziphelper.h"

FileUtil::FileUtil(QObject *parent) : QObject(parent)
{
    buf = new char[READ_BUFFER];
    unzipHelper = new UnzipHelper(this);
}

FileUtil::~FileUtil()
{
    delete[] buf;
}

/* Throws EmuFrontException */
int FileUtil::scanFilePath(FilePathObject *fp,
    QStringList filters, DbMediaImageContainer *dbMic,
    QProgressDialog *progressDialog)
{
    if (!fp->getSetup()){
        throw EmuFrontException(tr("Setup not available with %1.").arg(fp->getName()));
    }
    else if(!fp->getSetup()->getPlatform()){
        throw EmuFrontException(tr("No platform object available with %1.")
            .arg(fp->getSetup()->getName()));
    }
    else if (!fp->getSetup()->getMediaType()){
        throw EmuFrontException(tr("No media type available with %1.")
            .arg(fp->getSetup()->getName()));
    }

    int count = 0;
    /*qDebug() << QString("We have a platform %1, media type %2")
        .arg(fp->getSetup()->getPlatform()->getName())
        .arg(fp->getSetup()->getMediaType()->getName());*/
    QDir dir(fp->getName());
    if (!dir.exists() || !dir.isReadable())
        throw EmuFrontException(tr("Directory %1 doesn't exists or isn't readable!").arg(fp->getName()));

    //qDebug() << QString("Scanning directory %1.").arg(fp->getName());
    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable);

    if (filters.count() > 0) dir.setNameFilters(filters);

    // we'll go through the filtered archive files...
    QFileInfoList list = dir.entryInfoList();
    //qDebug() << "We have " << list.count() << " files to go through.";
    QList<MediaImageContainer*> containers;
    try {
        progressDialog->setMinimum(0);
        progressDialog->setMaximum(list.size());
        for (int i = 0; i < list.size(); ++i)
        {
            progressDialog->setValue(i);
            if (progressDialog->wasCanceled())
                break;
            QFileInfo fileInfo = list.at(i);
            //qDebug() << QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.absoluteFilePath());

            //... and collect the contents of each archive
            QMap<QString, EmuFrontObject*> files = unzipHelper->listContents(fileInfo.absoluteFilePath(), fp);

            if (files.count() > 0)
            {
                // read crc32 checksum for media image container
                quint32 crc = readCrc32(fileInfo.absoluteFilePath());
                FilePathObject *fpo = new FilePathObject(*fp);
                MediaImageContainer *con = new MediaImageContainer (
                        fileInfo.fileName(),
                        QString("%1").arg(crc, 0, 16),
                        fileInfo.size(),
                        files,
                        fpo // we need a copy since MediaImageContainers are deleted and the original filepath object would get deleted also.
                        );
                containers.append(con);
                ++count;
                //qDebug() << "We have " << containers.count() << " containers.";

                if (containers.count() >= MIC_BUFFER_SIZE)  {
                    //qDebug() << "We have " << containers.count() << " containers .. storing to db.";
                    showDbUpdating(progressDialog);
                    dbMic->storeContainers(containers, fp);
                    hideDbUpdating(progressDialog);
                    qDeleteAll(containers);
                    containers.clear();
                    //qDebug() << "containers now: " << containers.count();
                }
                //qDebug() << "We have " << containers.size() << " containers.";
            } // files.count() > 0
            else {
                qDebug() << "No files from container " << fileInfo.absoluteFilePath();
            }
        }
        progressDialog->setValue(list.size());
        if (containers.count() > 0) {
            //qDebug() << "Storing the rest " << containers.count() << " containers.";
            //emit dbUpdateInProgress();
            showDbUpdating(progressDialog);
            dbMic->storeContainers(containers, fp);
            hideDbUpdating(progressDialog);
            //emit dbUpdateFinished();
            qDeleteAll(containers);
            containers.clear();
 } } catch (EmuFrontException &e) { qDebug() << "Got exception " << e.what() << ", aborting & deleting created data objects.";
        qDeleteAll(containers);
        throw e;
    }
    //qDebug() << "Done scanning files!";
    return count;
}

/* Uses crc32 from zlib.h to count crc32 checksum value */
/* Throws EmuFrontException */
quint32 FileUtil::readCrc32(QString filePath)
{
    // todo ... use some crc32 tool for this ... or maybe use md5 or something like that!!!
    QFile file(filePath);
    //qDebug() << "readCrc32: " << filePath;
    if (!file.open(QIODevice::ReadOnly)) {
        throw EmuFrontException(QString(tr("Failed opening file %1 for reading the checksum!")).arg(filePath));
    }
    quint32 crc = crc32(0L, Z_NULL, 0);
    int read = 0;
    while((read = file.read(buf, READ_BUFFER))) {
        crc = crc32(crc, (const Bytef*) buf, read);
    }
    file.close();
    if (crc <= 0)
        throw EmuFrontException(QString(tr("Failed reading crc checksum for file %1!")).arg(filePath));
    //qDebug() << QString("readCrc32, crc: %1").arg(crc, 0, 16);
    return crc;
}

bool FileUtil::isSupportedFile(const QString filename, const QStringList supportedFileExtensions)
{
    QString ext = filename.section('.', -1);
    return supportedFileExtensions.contains(ext, Qt::CaseInsensitive);
}

void FileUtil::showDbUpdating(QProgressDialog *progressDialog)
{
    qDebug() << "DB updating";
    // TODO: the following is not currently working
    progressDialog->setWindowTitle(tr("Updating DB... please wait!"));
    progressDialog->setEnabled(false);
}

void FileUtil::hideDbUpdating(QProgressDialog *progressDialog)
{
    qDebug() << "DB update finished";
    // TODO: the following is not currently working
    progressDialog->setEnabled(true);
    progressDialog->setWindowTitle(tr("Scanning files"));
}


#include <QDir>
#include <QDebug>
#include "fileutil.h"
#include "OSDaB-Zip/unzip.h"
#include "../exceptions/emufrontexception.h"
#include "../dataobjects/setup.h"
#include "../dataobjects/mediaimage.h"
#include "../dataobjects/mediaimagecontainer.h"

FileUtil::FileUtil(QObject *parent) : QObject(parent)
{}

// TODO: fp is missing setup object reference!
QList<MediaImageContainer*> FileUtil::scanFilePath(const FilePathObject *fp, QStringList filters)
{
    QList<MediaImageContainer*> containers;
    QDir dir(fp->getName());
    if (!dir.exists() || !dir.isReadable())
        throw EmuFrontException(tr("Directory %1 doesn't exists or isn't readable!").arg(fp->getName()));

    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable);

    if (filters.count() > 0) dir.setNameFilters(filters);

    // we'll go through the filtered archive files...
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        qDebug() << QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.absoluteFilePath());

        //... and collect the contents of each archive
        QList<MediaImage*> files = listContents(fileInfo.absoluteFilePath(), fp);

        if (files.count() > 0)
        {
            MediaImageContainer *con = new MediaImageContainer
                (
                    fileInfo.fileName(),
                    "" /* TODO */,
                    fileInfo.size(),
                    files,
                    // TODO: is it guaranteed, that the file path object containing the setup object remains alive
                    // the whole lifecycle of (this) media image container object?
                    // * if we assign a copy of the setup object -> waste of memory and time
                    // * this function is designed to be used from media image path main dialog
                    //   where we can ensure the lifecycle of file path object -> maybe move the implementation there!?
                    fp->getSetup()
                );
            containers.append(con);
        }
    }
    return containers;
}

QList<MediaImage*> FileUtil::listContents(const QString filePath, const FilePathObject *fp)
{

    UnZip uz;
    UnZip::ErrorCode ec = uz.openArchive(filePath);
    if (ec != UnZip::Ok)
        throw EmuFrontException(tr("Error while opening zip-file %1, error code %2").arg(filePath).arg(ec));

    Setup *sup = fp->getSetup();
    QList<UnZip::ZipEntry> list = uz.entryList();
    QList<MediaImage*>  fileList;
    foreach(UnZip::ZipEntry entry, list)
    {
        if (isSupportedFile(entry.filename, sup->getSupportedFileTypeExtensions()))
        {
            QString checksum = QString("%1").arg(entry.crc32, 0, 16);
            MediaImage *effo = new MediaImage(entry.filename,
                checksum, entry.uncompressedSize);
            fileList << effo;
        }
    }

    return fileList;

}

bool FileUtil::isSupportedFile(const QString filename, const QStringList supportedFileExtensions)
{
    QString ext = filename.section('.', -1);
    return supportedFileExtensions.contains(ext, Qt::CaseInsensitive);
}

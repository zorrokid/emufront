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

        // if the archive contained supported file types we will create a media image container object
        // - platform
        // - meditype
        // - media image objects in a list
        // - time created / updated
        if (files.count() > 0)
        {
            MediaImageContainer *con = new MediaImageContainer();
            containers.append(con);
        }

        /*foreach (EmuFrontFileObject *o, files)
        {
            qDebug() << o->getFilename();
        }*/
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
        qDebug() << "We have an entry " << entry.filename
            << "with crc32 " << entry.crc32
            << " and file size " << entry.uncompressedSize;

        if (isSupportedFile(entry.filename, sup->getSupportedFileTypeExtensions()))
        {
            MediaImage *effo = new MediaImage();
            fileList << effo;
        }
    }

    return fileList;

}

bool FileUtil::isSupportedFile(const QString filename, const QStringList supportedFileExtensions)
{
    QString ext = filename.section('.', -1);
    return (supportedFileExtensions.indexOf(ext) > -1);
}

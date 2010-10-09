#include <QDir>
#include <QDebug>
#include "fileutil.h"
#include "zlib.h" /* crc32 */
#include "OSDaB-Zip/unzip.h"
#include "../exceptions/emufrontexception.h"
#include "../dataobjects/setup.h"
#include "../dataobjects/mediaimage.h"
#include "../dataobjects/mediaimagecontainer.h"
#include "../dataobjects/mediatype.h"
#include "../dataobjects/platform.h"

FileUtil::FileUtil(QObject *parent) : QObject(parent)
{
    buf = new char[READ_BUFFER];
}

FileUtil::~FileUtil()
{
    delete[] buf;
}

/* Throws EmuFrontException */
QList<MediaImageContainer*> FileUtil::scanFilePath(FilePathObject *fp, QStringList filters)
{
    if (!fp->getSetup()){
        throw EmuFrontException(tr("Setup not available with %1.").arg(fp->getName()));
    }
    else if(!fp->getSetup()->getPlatform()){
        throw EmuFrontException(tr("No platform object available with %1.")
            .arg(fp->getSetup()->getName()));
    }
    else if (!fp->getSetup()->getMediaType()){
        throw new EmuFrontException(tr("No media type available with %1.")
            .arg(fp->getSetup()->getName()));
    }
    qDebug() << QString("We have a platform %1, media type %2")
        .arg(fp->getSetup()->getPlatform()->getName())
        .arg(fp->getSetup()->getMediaType()->getName());
    QList<MediaImageContainer*> containers;
    QDir dir(fp->getName());
    if (!dir.exists() || !dir.isReadable())
        throw EmuFrontException(tr("Directory %1 doesn't exists or isn't readable!").arg(fp->getName()));

    qDebug() << QString("Scanning directory %1.").arg(fp->getName());
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
            quint32 crc = readCrc32(fileInfo.absoluteFilePath());
            MediaImageContainer *con = new MediaImageContainer
                (
                    fileInfo.fileName(),
                    QString("%1").arg(crc, 0, 16),
                    fileInfo.size(),
                    files,
                    new FilePathObject(*fp)
                );
            containers.append(con);
            qDebug() << "We have " << containers.size() << " containers.";
        }
    }
    qDebug() << "Done scanning files!";
    return containers;
}

/* Uses crc32 from zlib.h to count crc32 checksum value */
quint32 FileUtil::readCrc32(QString filePath)
{
    QFile file(filePath);
    qDebug() << "readCrc32: " << filePath;
    if (!file.open(QIODevice::ReadOnly)) {
        throw new EmuFrontException(QString(tr("Failed opening file %1 for reading the checksum!")).arg(filePath));
    }
    quint32 crc = crc32(0L, Z_NULL, 0);
    int read = 0;
    while((read = file.read(buf, READ_BUFFER))) {
        crc = crc32(crc, (const Bytef*) buf, read);
    }
    file.close();
    if (crc <= 0)
        throw new EmuFrontException(QString(tr("Failed reading crc checksum for file %1!")).arg(filePath));
    qDebug() << QString("readCrc32, crc: %1").arg(crc, 0, 16);
    return crc;
}

QList<MediaImage*> FileUtil::listContents(const QString filePath, const FilePathObject *fp)
{

    UnZip uz;
    UnZip::ErrorCode ec = uz.openArchive(filePath);
    if (ec != UnZip::Ok)
        throw EmuFrontException(tr("Error while opening zip-file %1, error code %2").arg(filePath).arg(ec));

    if (!fp->getSetup()){
        throw EmuFrontException(tr("Setup not available with %1.").arg(fp->getName()));
    }

    Setup *sup = fp->getSetup();
    QList<UnZip::ZipEntry> list = uz.entryList();
    QList<MediaImage*>  fileList;
    foreach(UnZip::ZipEntry entry, list)
    {
        qDebug() << "Zip entry " << entry.filename;
        if (isSupportedFile(entry.filename, sup->getSupportedFileTypeExtensions()))
        {
            QString checksum = QString("%1").arg(entry.crc32, 0, 16);
            qDebug() << "Checksum " << checksum;
            MediaImage *effo = new MediaImage(entry.filename,
                checksum, entry.uncompressedSize);
            fileList << effo;
        }
    }

    qDebug() << "File list has " << fileList.size() << " entries.";
    return fileList;

}

bool FileUtil::isSupportedFile(const QString filename, const QStringList supportedFileExtensions)
{
    QString ext = filename.section('.', -1);
    return supportedFileExtensions.contains(ext, Qt::CaseInsensitive);
}

#include <QDir>
#include <QDebug>
#include "fileutil.h"
#include "OSDaB-Zip/unzip.h"

FileUtil::FileUtil(QObject *parent) : QObject(parent)
{}

QList<EmuFrontFileObject*> FileUtil::scanFilePath(const FilePathObject *fp, QStringList filters)
{
    QList<EmuFrontFileObject*> files;
    qDebug() << "Will scan file path " << fp->getName();
    QDir dir(fp->getName());
    if (!dir.exists() || !dir.isReadable())
        throw QString(tr("Directory %1 doesn't exists or isn't readable!").arg(fp->getName()));

    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable);

    if (filters.count() > 0) dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        qDebug() << QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.absoluteFilePath());

        QList<EmuFrontFileObject*> files = listContents(fileInfo.absoluteFilePath(), fp);

        foreach (EmuFrontFileObject *o, files)
        {
            qDebug() << o->getFilename();
        }
    }
    return files;
}

QList<EmuFrontFileObject*> FileUtil::listContents(const QString filePath, const FilePathObject *fp)
{
    UnZip uz;
    UnZip::ErrorCode ec = uz.openArchive(filePath);
    if (ec != UnZip::Ok)
        throw QString("Error while opening zip-file %1, error code %2").arg(filePath).arg(ec);
    QList<UnZip::ZipEntry> list = uz.entryList();
    foreach(UnZip::ZipEntry entry, list)
    {
        qDebug() << "We have an entry " << entry.filename;
    }
    QList<EmuFrontFileObject*>  fileList;

    return fileList;

}

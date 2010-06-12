#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <QObject>
#include "../dataobjects/emufrontfileobject.h"
#include "../dataobjects/filepathobject.h"

class QStringList;
class MediaImageContainer;
class MediaImage;

class FileUtil : QObject
{


public:
    FileUtil(QObject *parent);

    QList<MediaImageContainer*> scanFilePath(const FilePathObject *fpo, const QStringList filters);
private:
    QList<MediaImage*>  listContents(const QString filePath, const FilePathObject *fp);
    bool isSupportedFile(const QString filename, const QStringList supportedFileExtensions);
};

#endif // FILEUTIL_H

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

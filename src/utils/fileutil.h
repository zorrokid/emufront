#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <QObject>
#include "../dataobjects/emufrontfileobject.h"
#include "../dataobjects/filepathobject.h"

class QStringList;

class FileUtil : QObject
{


public:
    FileUtil(QObject *parent);

    QList<EmuFrontFileObject*> scanFilePath(const FilePathObject *fpo, const QStringList filters);
private:
    QList<EmuFrontFileObject*>  listContents(const QString filePath, const FilePathObject *fp);
};

#endif // FILEUTIL_H

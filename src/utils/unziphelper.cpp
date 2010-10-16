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

#include <QFile>
#include <QDebug>
#include "unziphelper.h"
#include "../dataobjects/mediaimage.h"
#include "../dataobjects/filepathobject.h"
#include "../exceptions/emufrontexception.h"

const QString UnzipHelper::UNZIP_COMMAND = "unzip ";
const QString UnzipHelper::UNZIP_LIST_ARGS = "-lv ";

UnzipHelper::UnzipHelper(QObject *parent) :
    ProcessHelper(parent)
{
}

QList<MediaImage*> UnzipHelper::listContents(const QString filePath, const FilePathObject *fp)
{
    if (!fp->getSetup()){
        throw EmuFrontException(tr("Setup not available with %1.").arg(fp->getName()));
    }

    QFile fl(filePath);
    if (!fl.open(QIODevice::ReadOnly)) {
        throw new EmuFrontException(tr("Couldn't read file %1.").arg(filePath));
    }

    Setup *sup = fp->getSetup();
    QList<MediaImage*>  fileList;

    QProcess proc(this);
    QString command;
    command.append(UNZIP_COMMAND);
    command.append(UNZIP_LIST_ARGS);
    command.append("\"");
    command.append(filePath);
    command.append("\"");
    qDebug() << command;
    proc.start(command);
    // TODO: slot(s) for (start and) error signal(s)
    bool procOk = proc.waitForFinished();
    if (!procOk) {
        throw new EmuFrontException(tr("Listing information from file %1 failed with unzip.").arg(filePath));
    }
    QString err = proc.readAllStandardError();
    QString msg = proc.readAllStandardOutput();
    qDebug() << "\nErrors:\n" << err << "\nMessage:\n" << msg;

    /*

    The unzip output should have 8 columns, we need to collect the data from
    size, crc-32 and name columns.

    $ unzip -lv zak.zip
    Archive:  zak.zip
     Length   Method    Size  Cmpr    Date    Time   CRC-32   Name
    --------  ------  ------- ---- ---------- ----- --------  ----
      174848  Defl:N    21936  88% 1996-12-24 23:32 cd68329c  Zak McKracken and the Alien Mindbenders (1988)(Lucasfilm Games)(Disk 1 of 2 Side A)(Boot).d64
      174848  Defl:N    21949  87% 1996-12-24 23:32 dc0d89f8  Zak McKracken and the Alien Mindbenders (1988)(Lucasfilm Games)(Disk 1 of 2 Side A)(Boot)[a].d64
      174848  Defl:N    81818  53% 1996-12-24 23:32 a11bc616  Zak McKracken and the Alien Mindbenders (1988)(Lucasfilm Games)(Disk 1 of 2 Side A)(Boot)[cr ESI].d64
      174848  Defl:N    48833  72% 1996-12-24 23:32 0815053d  Zak McKracken and the Alien Mindbenders (1988)(Lucasfilm Games)(Disk 1 of 2 Side A)(Boot)[cr SCI].d64
      174848  Defl:N   105964  39% 1996-12-24 23:32 0c943d80  Zak McKracken and the Alien Mindbenders (1988)(Lucasfilm Games)(Disk 1 of 2 Side A)[cr Ikari].d64
      174848  Defl:N    17876  90% 1996-12-24 23:32 51397eb8  Zak McKracken and the Alien Mindbenders (1988)(Lucasfilm Games)(Disk 1 of 2 Side B)[cr SCI].d64
      174848  Defl:N   106231  39% 1996-12-24 23:32 0efadb0a  Zak McKracken and the Alien Mindbenders (1988)(Lucasfilm Games)(Disk 2 of 2 Side A).d64
      174848  Defl:N   105974  39% 1996-12-24 23:32 6935c3e7  Zak McKracken and the Alien Mindbenders (1988)(Lucasfilm Games)(Disk 2 of 2 Side A)[a].d64
      174848  Defl:N   105963  39% 1996-12-24 23:32 1e9c31de  Zak McKracken and the Alien Mindbenders (1988)(Lucasfilm Games)(Disk 2 of 2 Side A)[cr ESI].d64
      174848  Defl:N    26294  85% 1996-12-24 23:32 ba5fdfdd  Zak McKracken and the Alien Mindbenders (1988)(Lucasfilm Games)(Disk 2 of 2 Side A)[cr Ikari].d64
      174848  Defl:N   117996  33% 1996-12-24 23:32 efbf3fd6  Zak McKracken and the Alien Mindbenders (1988)(Lucasfilm Games)(Disk 2 of 2 Side B).d64
      174848  Defl:N   118015  33% 1996-12-24 23:32 c9541ecd  Zak McKracken and the Alien Mindbenders (1988)(Lucasfilm Games)(Disk 2 of 2 Side B)[a].d64
      174848  Defl:N   118010  33% 1996-12-24 23:32 68341056  Zak McKracken and the Alien Mindbenders (1988)(Lucasfilm Games)(Disk 2 of 2 Side B)[cr ESI].d64
    --------          -------  ---                            -------
     2273024           996859  56%                            13 files

     Here's a regex tested in VIM matching an entry line
     /^\s\+\d\+\s\+[A-Za-z:]*\s\+\d\+\s\+\d\{1,3}%\s\+\d\{4}-\d\{2}-\d\{2}\s\+\d\{2}:\d\{2}\s\+[0-9a-f]\{8}\s\+.\+$

     Here's a regex (tested in VIM) picking the three required fields, length, crc-32 and filename:
     :%s/^\s\+\(\d\+\)\s\+[A-Za-z:]*\s\+\d\+\s\+\d\{1,3}%\s\+\d\{4}-\d\{2}-\d\{2}\s\+\d\{2}:\d\{2}\s\+\([0-9a-f]\{8}\)\s\+\(.*$\)/\1 \2 \3/gc
     */
    QStringList lines = msg.split('\n'
        //QRegExp("^\\s+\\d+\\s+[A-Za-z:]*\\s+\\d+\\s+\\d{1,3}%\\s+\\d{4}-\\d{2}-\\d{2}\\s+\\d{2}:\\d{2}\\s+[0-9a-f]{8}\\s+.+$")
        );
    QStringList entries;
    QRegExp test("^\\s+\\d+\\s+[A-Za-z:]*\\s+\\d+\\s+\\d{1,3}%\\s+\\d{4}-\\d{2}-\\d{2}\\s+\\d{2}:\\d{2}\\s+[0-9a-f]{8}\\s+.+$");
    QRegExp regExEntries("^\\s+(\\d+)\\s+[A-Za-z:]*\\s+\\d+\\s+\\d{1,3}%\\s+\\d{4}-\\d{2}-\\d{2}\\s+\\d{2}:\\d{2}\\s+([0-9a-f]{8})\\s+(\\S.+)$");
    foreach(QString ln, lines) {
        if (!test.exactMatch(ln)) continue;
        int pos = regExEntries.indexIn(ln);
        entries = regExEntries.capturedTexts();
        if (entries.count() < 4) continue;
        QString filename = entries[3];
        QString checksum = entries[2];
        QString lenStr = entries[1];
        bool ok;
        int length = lenStr.toInt(&ok);
        if (!ok) continue;
        MediaImage *effo = new MediaImage(filename, checksum, length);
        fileList << effo;
    }

    /*UnZip uz;
    UnZip::ErrorCode ec = uz.openArchive(filePath);
    if (ec != UnZip::Ok)
        throw EmuFrontException(tr("Error while opening zip-file %1, error code %2").arg(filePath).arg(ec));

    QList<UnZip::ZipEntry> list = uz.entryList();
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
    }*/

    qDebug() << "File list has " << fileList.size() << " entries.";
    return fileList;

}


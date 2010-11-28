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

#ifndef MEDIAIMAGEPATHMAINDIALOG_H
#define MEDIAIMAGEPATHMAINDIALOG_H

#include "dbobjectdialog.h"
#include "../db/dbmediaimagecontainer.h"

class FileUtil;
class QProgressDialog;

class MediaImagePathMainDialog : public DbObjectDialog
{
    Q_OBJECT

public:
    MediaImagePathMainDialog(QWidget *parent);
    ~MediaImagePathMainDialog();

protected:
    virtual void initEditDialog();
    virtual void deleteCurrentObject();
    virtual EmuFrontObject* createObject();
    virtual void connectSignals();
    virtual void cleanUp();

private slots:
    void beginScanFilePath();

private:
    QPushButton *scanButton;
    DbMediaImageContainer *dbMediaImageContainer;
    FileUtil *fileUtil;
    QProgressDialog *progressDialog;

    //  QString and QStringList are implicitly shared
    void scanFilePath(const QString path, const QStringList filters);
    void initProgressDialog();
};

#endif // MEDIAIMAGEPATHMAINDIALOG_H

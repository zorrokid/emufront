/*
** EmuFront
** Copyright 2010-2011 Mikko Keinänen
**
** This file is part of EmuFront.
**
**
** EmuFront is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License version 2 as published by
** the Free Software Foundation and appearing in the file gpl.txt included in the
** packaging of this file.
**
** EmuFront is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FILEPATHEDITVIEW_H
#define FILEPATHEDITVIEW_H

#include "emufronteditview.h"

class FileUtil;
class QProgressDialog;
class SetupModel;

class FilePathEditView : public EmuFrontEditView
{
    Q_OBJECT
public:
    FilePathEditView(SetupModel *supModel, QWidget *parent = 0);

protected:
    virtual void connectSignals();

private slots:
    void beginScanFilePath();

private:
    QPushButton* scanButton;
    FileUtil *fileUtil;
    QProgressDialog *progressDialog;
    void scanFilePath(const QString path, const QStringList filters);
    void initProgressDialog();
    virtual void setHiddenColumns();
};

#endif // FILEPATHEDITVIEW_H

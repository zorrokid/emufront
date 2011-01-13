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
*/#ifndef BROWSEFILEPATHDIALOG_H
#define BROWSEFILEPATHDIALOG_H

#include "dataobjecteditdialog.h"

class QLabel;
class QPushButton;

class BrowseFilePathDialog : public DataObjectEditDialog
{
    Q_OBJECT
public:
    BrowseFilePathDialog(QWidget *parent, EmuFrontObject*,
        Qt::Orientation orientation = Qt::Horizontal);

signals:
    void filePathUpdated();

protected slots:
    void browseFilePath();

protected:
    void connectSignals();
    void initWidgets();
    virtual void clear();
    QLabel *filePathLabel;
    QPushButton *filePathButton;
};

#endif // BROWSEFILEPATHDIALOG_H

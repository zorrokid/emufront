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

#include <QtGui>
#include "browsefilepathdialog.h"

BrowseFilePathDialog::BrowseFilePathDialog(QWidget *parent, EmuFrontObject *efo, Qt::Orientation orientation) :
    DataObjectEditDialog(parent, efo, orientation)
{
}

void BrowseFilePathDialog::connectSignals()
{
    DataObjectEditDialog::connectSignals();
    connect(filePathButton, SIGNAL(clicked()), this, SLOT(browseFilePath()));
}

void BrowseFilePathDialog::initWidgets()
{
    filePathLabel = new QLabel;
    filePathButton = new QPushButton(tr("&Browse filepath"));
}

void BrowseFilePathDialog::browseFilePath()
{
    QString startPath = (efObject && !efObject->getName().isEmpty())
        ? efObject->getName()
        : QDir::homePath();

    QString fpath = QFileDialog::getExistingDirectory(this,
        tr("Select a directory"), startPath,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir d(fpath);
    if (d.exists() && d.isReadable()) {
        filePathLabel->setText(d.path());
        if (d.path() != startPath) emit filePathUpdated();
    }
}

void BrowseFilePathDialog::clear()
{
    filePathLabel->clear();
}

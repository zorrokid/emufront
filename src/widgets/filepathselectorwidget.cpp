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
#include <QtGui>
#include "filepathselectorwidget.h"

FilePathSelectorWidget::FilePathSelectorWidget(QWidget *parent) :
    QWidget(parent)
{
    filePathLabel = new QLabel;
    filePathButton = new QPushButton(tr("&Browse"));
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(filePathButton);
    layout->addWidget(filePathLabel);
    setLayout(layout);
    connectSignals();
}

void FilePathSelectorWidget::connectSignals()
{
    connect(filePathButton, SIGNAL(clicked()), this, SLOT(browseFilePath()));
}

void FilePathSelectorWidget::browseFilePath()
{
    qDebug() << "FilePathSelectorWidget::browseFilePath()";
    QString startPath = filePathLabel->text().isEmpty() ?
        QDir::homePath() : filePathLabel->text();

    QString fpath = QFileDialog::getExistingDirectory(this,
        tr("Select a directory"), startPath,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            QDir d(fpath);

    if (d.exists() && d.isReadable()) {
        filePathLabel->setText(d.path());
        if (d.path() != startPath)
            emit filePathUpdated();
    }
}

void FilePathSelectorWidget::setFilePath(QString &path)
{
    filePathLabel->setText(path);
}

QString FilePathSelectorWidget::getFilePath() const
{
    return filePathLabel->text();
}

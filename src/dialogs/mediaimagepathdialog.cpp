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
#include <QSqlTableModel>
#include <QSqlRecord>
#include "mediaimagepathdialog.h"
#include "../db/dbsetup.h"
#include "../dataobjects/filepathobject.h"
#include "../widgets/setupcombobox.h"

MediaImagePathDialog::MediaImagePathDialog(QWidget *parent, EmuFrontObject *efObject)
    : BrowseFilePathDialog(parent, efObject, Qt::Horizontal)
{
    initWidgets();
    dbPlatform = 0;
    dbMediaType = 0;
    dbSetup = 0;
    connectSignals();
    layout();
}

MediaImagePathDialog::~MediaImagePathDialog()
{
}

/*void MediaImagePathDialog::connectSignals()
{
    DataObjectEditDialog::connectSignals();
    connect(filePathButton, SIGNAL(clicked()), this, SLOT(browseFilePath()));
}*/

/*void MediaImagePathDialog::browseFilePath()
{
    QString fpath = QFileDialog::getExistingDirectory(this, tr("Select a directory"), ".",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir d(fpath);
    if (d.exists() && d.isReadable())
    {
        filePathLabel->setText(d.path());
    }
}*/

void MediaImagePathDialog::initWidgets()
{
    BrowseFilePathDialog::initWidgets();
    // these widgets will be automatically parented using layout components
    //filePathLabel = new QLabel;
    //filePathButton = new QPushButton(tr("&Browse filepath"));
    dbSetup = new DbSetup(this);
    setupComBox = new SetupComboBox(dbSetup, this);
}

void MediaImagePathDialog::layout()
{
   QLabel *setupLabel = new QLabel(tr("&Set up"));
   setupLabel->setBuddy(setupComBox);

   QGridLayout *gridLayout = new QGridLayout;
   gridLayout->addWidget(setupLabel, 0, 0);
   gridLayout->addWidget(setupComBox, 0, 1);
   gridLayout->addWidget(filePathButton, 1, 0);
   gridLayout->addWidget(filePathLabel, 1, 1);
   QVBoxLayout *mainLayout = new QVBoxLayout;
   mainLayout->addLayout(gridLayout);
   mainLayout->addWidget(buttonBox);
   setLayout(mainLayout);

   setWindowTitle(tr("Set media image paths"));
}

void MediaImagePathDialog::setDataObject(EmuFrontObject *ob)
{
    if (!ob) return;
    efObject = ob;
    FilePathObject *fpo = dynamic_cast<FilePathObject*>(ob);
    QString fpath = fpo->getName();
    filePathLabel->setText(fpath);
    if (fpo->getSetup()) setSelectedSetup(fpo->getSetup());
}

void MediaImagePathDialog::setSelectedSetup(const Setup *sup)
{
    setupComBox->setSelected(sup);
}

Setup* MediaImagePathDialog::getSelectedSetup()
{
    EmuFrontObject *ob = 0;

    try {
        ob = setupComBox->getSelected();
    }
    catch(EmuFrontException &e){
        errorMessage->showMessage(e.what());
    }

    if (!ob) return 0;
    return dynamic_cast<Setup*>(ob);
}

void MediaImagePathDialog::acceptChanges()
{
    FilePathObject *fpo = dynamic_cast<FilePathObject*>(efObject);
    Setup *sup = getSelectedSetup();
    if (!sup)
    {
        QMessageBox::information(this, tr("Set up"), tr("Set up not selected"), QMessageBox::Ok);
        return;
    }
    bool change = false;
    qDebug() << "Setup selected " << sup->getName();
    QString filePath = filePathLabel->text();
    if (filePath.isEmpty())
    {
        QMessageBox::information(this, tr("File path"), tr("File path was not selected"), QMessageBox::Ok);
        return;
    }
    if (filePath != fpo->getName()) {
        fpo->setName(filePath);
        change = true;
    }

    Setup *tmp = fpo->getSetup();

    if (!tmp || *sup != *tmp)
    {
        delete tmp;
        fpo->setSetup(sup);
        change = true;
    }
    if (change) emit dataObjectUpdated();
    efObject = 0;
    close();
}


void MediaImagePathDialog::updateData()
{
    setupComBox->updateDataModel();
}

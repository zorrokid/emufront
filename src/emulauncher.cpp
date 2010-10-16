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
#include <QProcess>
#include <QSqlTableModel>
#include <QItemSelectionModel>
#include "emulauncher.h"
#include "db/dbmediatype.h"
#include "db/dbplatform.h"
#include "db/dbexecutable.h"
#include "db/dbmediaimagecontainer.h"
#include "widgets/effileobjectcombobox.h"
#include "widgets/executablecombobox.h"
#include "dataobjects/executable.h"
#include "utils/emuhelper.h"

EmuLauncher::EmuLauncher(QWidget *parent) :
    QWidget(parent)
{
    dbPlatform = new DbPlatform(this);
    dbMediaType = new DbMediaType(this);
    dbExec = new DbExecutable(this);
    dbMic = 0;
    proc = 0;
    emuHelper = new EmuHelper(this);
    initWidgets();
    layout();
    connectSignals();
}

EmuLauncher::~EmuLauncher()
{
    if (proc) {
        proc->kill(); // TODO: do this in a more sophisticated way
        delete proc;
    }
}

void EmuLauncher::updateData()
{
    platformSelectBox->updateDataModel();
    mediaTypeSelectBox->updateDataModel();
    execSelectBox->updateDataModel();
}

void EmuLauncher::initWidgets()
{
    micTable = new QTableView(this);
    micTable->setSelectionMode(QAbstractItemView::MultiSelection);
    mediaTypeSelectBox = new EFFileObjectComboBox(dbMediaType, this);
    platformSelectBox = new EFFileObjectComboBox(dbPlatform, this);
    execSelectBox = new ExecutableComboBox(dbExec, this);
    selectButton = new QPushButton(tr("&Update"));
    launchButton = new QPushButton(tr("&Launch"));
}

void EmuLauncher::layout()
{
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(platformSelectBox, 0, 0);
    grid->addWidget(mediaTypeSelectBox, 1, 0);
    grid->addWidget(selectButton, 1, 1);
    grid->addWidget(micTable, 2, 0, 1, 2);
    grid->addWidget(execSelectBox, 3, 0);
    grid->addWidget(launchButton, 3, 1);
    setLayout(grid);
}

void EmuLauncher::connectSignals()
{
    connect(selectButton, SIGNAL(clicked()), this, SLOT(updateMediaImageContainers()));
    connect(launchButton, SIGNAL(clicked()),this, SLOT(launchEmu()));
    connect(emuHelper, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
    connect(emuHelper, SIGNAL(finished(int)), this, SLOT(processFinished(int)));
}

void EmuLauncher::updateMediaImageContainers()
{
    qDebug() << "updateMediaImageContainers slot";
    int mtid = mediaTypeSelectBox->getSelected()
        ? mediaTypeSelectBox->getSelected()->getId()
        : -1;
    int plfid = platformSelectBox->getSelected()
        ? platformSelectBox->getSelected()->getId()
        : -1;

    if (!dbMic) dbMic = new DbMediaImageContainer(this);
    dbMic->filter(mtid, plfid);
    micTable->setModel(dbMic->getDataModel());
    micTable->resizeColumnsToContents();
    platformSelectBox->updateDataModel();
    mediaTypeSelectBox->updateDataModel();
}

void EmuLauncher::launchEmu()
{
    try {
        if (!micTable || !micTable->model()) {
            throw EmuFrontException(tr("No search results available!"));
        }
        if (!execSelectBox || execSelectBox->currentIndex() == -1) {
            throw EmuFrontException(tr("Emulator not selected!"));
        }
        //QModelIndex mindex = micTable->currentIndex();
        QItemSelectionModel *selModel = micTable->selectionModel();
        QModelIndexList listMIndex =  selModel->selectedIndexes();
        /*if (!mindex.isValid()) {
            throw EmuFrontException(tr("Media image container not selected!"));
        }*/
        if (listMIndex.count() < 1) {
            throw EmuFrontException(tr("Media image container not selected!"));
        }
        qDebug() << listMIndex.count() << " items selected.";

        // TODO: multiple media image container selection
        //          - build a list of selected media image objects
        //          - check that the platform and media type (setup) matches
        QModelIndex mindex = listMIndex.first();
        EmuFrontObject *obImg = dbMic->getDataObjectFromModel(&mindex);
        if (!obImg) {
            throw EmuFrontException(tr("Failed fetching selected media image container."));
        }
        MediaImageContainer *mic = dynamic_cast<MediaImageContainer*>(obImg);
        if (!mic) {
            throw EmuFrontException(tr("Failed creating media image container object!"));
        }
        EmuFrontObject *obExe = execSelectBox->getSelected();
        if (!obExe) {
            throw EmuFrontException(tr("Failed fetching selected emulator!"));
        }
        Executable *exe = dynamic_cast<Executable*>(obExe);
        if (!exe) {
            throw EmuFrontException(tr("Failed creating Emulator object!"));
        }
        qDebug() << "Selected media image container "
                << mic->getName() << " and emulator "
                << obExe->getName() << ".";
        if (mic->getMediaImages().count() > 0) {
            // 1. Launch media image
            // TODO
            // 2. If 2 or more media images in container
            //    show a diaglog for choosing the boot image
            // 3. If 2 or more media image containers selected
            //    from a list show a dialog listing all the media
            //    images in those container for choosing the
            //    boot image
            // 4. If selected emulator command line containes more
            //    than one media image placeholder ($1, $2, ...)
            //    show a dialog for ordering the media images to
            //    right order.
            QList<MediaImage*> ls = mic->getMediaImages();
            foreach(MediaImage *mi, ls) {
                qDebug() << "Media image " << mi->getName();
            }
            emuHelper->launch(exe, mic);
        }
    } catch (EmuFrontException efe) {
        QMessageBox::information(this, tr("Launching emulator"),
                                 efe.what(), QMessageBox::Ok);
        return;
    }
}

void EmuLauncher::processError(QProcess::ProcessError e)
{
    cleanTmp();
    QString stdErr = emuHelper->readAllStandardError();
    QMessageBox::warning(this, tr("Emulator"),
        tr("Launching emulator failed with: %1.\n").arg(e)
        .append(";\n").append(proc->errorString().append(";\n")
        .append(stdErr)), QMessageBox::Ok );
}

/* Slot for EmuHelper process finished, clears the temporary folder files */
void EmuLauncher::processFinished(int a)
{
    cleanTmp();
    QString stdErr = emuHelper->readAllStandardError();
    QString stdMsg = emuHelper->readAllStandardOutput();
    QString msg = tr("Emulator has finished with: %1.\n").arg(a).append(stdMsg);
    if (a) msg.append("; ").append(proc->errorString()).append(";\n").append(stdErr);
    QMessageBox::information(this, tr("Emulator finished"), msg, QMessageBox::Ok);
}

void EmuLauncher::cleanTmp()
{
    // TODO
}

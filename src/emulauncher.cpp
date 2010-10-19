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
#include "dialogs/emufrontinputdialog.h"

EmuLauncher::EmuLauncher(QWidget *parent) :
    QWidget(parent)
{
    dbPlatform = new DbPlatform(this);
    dbMediaType = new DbMediaType(this);
    dbExec = new DbExecutable(this);
    dbMic = 0;
    emuHelper = new EmuHelper(this);
    initWidgets();
    layout();
    connectSignals();
}

EmuLauncher::~EmuLauncher()
{
    if (emuHelper) {
        qDebug() << "EmuLauncher destructor";
        if (emuHelper->state() == EmuHelper::Running)
            qDebug() << "EmuHelper process is running, killing...";
            emuHelper->kill();
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
    QMap<QString, EmuFrontObject*> mediaImages;
    QList<MediaImageContainer*> mediaImageContainers;
    Executable *exe;
    try {
        if (!micTable || !micTable->model()) {
            throw EmuFrontException(tr("No search results available!"));
        }
        if (!execSelectBox || execSelectBox->currentIndex() == -1) {
            throw EmuFrontException(tr("Emulator not selected!"));
        }
        QItemSelectionModel *selModel = micTable->selectionModel();
        QModelIndexList listMIndex =  selModel->selectedIndexes();
        if (listMIndex.count() < 1) {
            throw EmuFrontException(tr("Media image container not selected!"));
        }
        qDebug() << listMIndex.count() << " items selected.";

        EmuFrontObject *obExe = execSelectBox->getSelected();
        if (!obExe) {
            throw EmuFrontException(tr("Failed fetching selected emulator!"));
        }
        exe = dynamic_cast<Executable*>(obExe);
        if (!exe) {
            throw EmuFrontException(tr("Failed creating Emulator object!"));
        }

        foreach(QModelIndex mind, listMIndex) {
            if (!mind.isValid()) continue;
            EmuFrontObject *obImg = dbMic->getDataObjectFromModel(&mind);
            if (!obImg) {
                qDebug() << "Failed creating media image container at row " << mind.row();
                continue;
            }
            MediaImageContainer *mic = dynamic_cast<MediaImageContainer*>(obImg);
            if (!mic) {
                qDebug() << "Failed to create media image container for " << obImg->getName();
                delete obImg;
                continue;
            }
            mediaImageContainers << mic;
            QMap<QString, EmuFrontObject*> contained = mic->getMediaImages();
            mediaImages.unite(contained);
        }

        if (mediaImages.count() < 1) {
            throw EmuFrontException("No media images available!");
        }

        // check if command options have slots for nr media images > 1 e.g. "-diska $1 -diskb $2 ..."
        QString opts = exe->getOptions();
        QRegExp rx("(\\s\\$\\d+)");
        QStringList list;
        int pos = 0;
        while ((pos = rx.indexIn(opts, pos)) != -1) {
            list << rx.cap(1);
            pos += rx.matchedLength();
        }

        bool ok;
        QList<EmuFrontObject*> selectedImages;
        if (list.count() > mediaImages.count()) {
            throw EmuFrontException(tr("Select %1 media images for this emulator configuration").arg(list.count()));
        }
        if (list.count() > 1) {
            int lim = list.count() == mediaImages.count() ? list.count() - 1 : list.count();
            for(int i = 0; i < lim; i++) {
                EmuFrontObject *efo = EmuFrontInputDialog::getItem(
                        this, tr("Select image no. %1").arg(i+1), tr("Select"), mediaImages.values(), 0, false, &ok);
                if (!ok)  {
                    throw EmuFrontException(tr("Boot image selection was canceled, aborting."));
                }
                selectedImages << efo;
                MediaImage *mi = dynamic_cast<MediaImage*>(efo);
                QString key = mi->getCheckSum();
                mediaImages.remove(key);
            }
            if (mediaImages.count() == 1) {
                // there should be at least one media image left in mediaImages map
                selectedImages << mediaImages.values().first();
            }
        }
        else if (mediaImages.count() > 1) {
            // show select boot image dialog
            EmuFrontObject *efo = EmuFrontInputDialog::getItem(
                this, tr("Select boot image"), tr("Select"), mediaImages.values(), 0, false, &ok);
            if (!ok)  {
                throw EmuFrontException(tr("Boot image selection was canceled, aborting."));
            }
            selectedImages << efo;
        }
        else if (mediaImages.count() == 1)
            selectedImages << mediaImages.values().first();
        // in the both cases the (ordered) list of media images will be passed to emuHelper

        if (selectedImages.count() < 1)
            throw EmuFrontException(tr("No media images selected"));

        emuHelper->launch(exe, mediaImageContainers, selectedImages, list.count());
        micTable->clearSelection();
    } catch (EmuFrontException efe) {
        delete exe;
        qDeleteAll(mediaImageContainers);
        //qDeleteAll(mediaImages); these are already deleted along with containers
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
        .append(";\n").append(emuHelper->errorString().append(";\n")
        .append(stdErr)), QMessageBox::Ok );
}

/* Slot for EmuHelper process finished, clears the temporary folder files */
void EmuLauncher::processFinished(int a)
{
    cleanTmp();
    QString stdErr = emuHelper->readAllStandardError();
    QString stdMsg = emuHelper->readAllStandardOutput();
    QString msg = tr("Emulator has finished with: %1.\n").arg(a).append(stdMsg);
    if (a) msg.append("; ").append(emuHelper->errorString()).append(";\n").append(stdErr);
    QMessageBox::information(this, tr("Emulator finished"), msg, QMessageBox::Ok);
}

void EmuLauncher::cleanTmp()
{
    // TODO
}

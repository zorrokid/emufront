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

EmuLauncher::EmuLauncher(QErrorMessage *errorMessage, QWidget *parent, QString tmp) :
    QWidget(parent), tmpDirPath(tmp), errorMessage(errorMessage)
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
    //execSelectBox->updateDataModel();
}

void EmuLauncher::initWidgets()
{
    micTable = new QTableView(this);
    micTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    micTable->setCornerButtonEnabled(false);
    micTable->verticalHeader()->setVisible(false);
    //micTable->horizontalHeader()->setDisabled(true);
    micTable->horizontalHeader()->setClickable(false);
    mediaTypeSelectBox = new EFFileObjectComboBox(dbMediaType, this);
    platformSelectBox = new EFFileObjectComboBox(dbPlatform, this);
    execSelectBox = new ExecutableComboBox(dbExec, this);
    selectButton = new QPushButton(tr("&Update"), this);
    launchButton = new QPushButton(tr("&Launch"), this);
}

void EmuLauncher::layout()
{
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(platformSelectBox, 0, 0);
    grid->addWidget(mediaTypeSelectBox, 0, 1);
    grid->addWidget(selectButton, 0, 2);
    grid->setColumnStretch(3, 1);

    grid->addWidget(micTable, 1, 0, 1, 4);
    grid->addWidget(execSelectBox, 2, 0);
    grid->addWidget(launchButton, 2, 1);
    // grid will be implicitily parented to this
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
    int mtid, plfid = -1;
    MediaType *mt = 0;
    Platform *plf = 0;
    try {
        mt = dynamic_cast<MediaType*>(mediaTypeSelectBox->getSelected());
        plf = dynamic_cast<Platform*>(platformSelectBox->getSelected());
    }
    catch(EmuFrontException &e){
        errorMessage->showMessage(e.what());
        return;
    }
    mtid = mt ? mt->getId() : -1;
    plfid = plf ? plf->getId() : -1;
    if (mt) delete mt;
    if (plf) delete plf;

    if (!dbMic) dbMic = new DbMediaImageContainer(this);
    dbMic->filter(mtid, plfid);
    micTable->setModel(dbMic->getDataModel());
    micTable->hideColumn(DbMediaImageContainer::MIC_FileId);
    micTable->hideColumn(DbMediaImageContainer::MIC_FileSize);
    micTable->hideColumn(DbMediaImageContainer::MIC_FileCheckSum);
    micTable->hideColumn(DbMediaImageContainer::MIC_FilePathId);
    micTable->hideColumn(DbMediaImageContainer::MIC_FilePathName);
    micTable->hideColumn(DbMediaImageContainer::MIC_SetupId);
    micTable->hideColumn(DbMediaImageContainer::MIC_PlatformName);
    micTable->hideColumn(DbMediaImageContainer::MIC_PlatformId);
    micTable->hideColumn(DbMediaImageContainer::MIC_MediaTypeName);
    micTable->hideColumn(DbMediaImageContainer::MIC_MediaTypeId);
    micTable->resizeColumnsToContents();
    platformSelectBox->updateDataModel();
    mediaTypeSelectBox->updateDataModel();
    execSelectBox->updateToSetup(plfid, mtid);
}

void EmuLauncher::launchEmu()
{
    // if selected emulator has no extensions configured, it's assumed to be a M.A.M.E. or similar and
    // map of media images will be no be used
    QMap<QString, EmuFrontObject*> mediaImages;
    QList<MediaImageContainer*> mediaImageContainers;
    Executable *exe = 0;
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

        qDebug() << "File types; " << exe->getSetup()->getSupportedFileTypeExtensions().count();

        bool mame = exe->getSetup()->getSupportedFileTypeExtensions().isEmpty();

        if (mame && listMIndex.count() > 1) {
            throw EmuFrontException(tr("No supported file types configured for this emulator configuration. "
                "Assuming emulator support container files as is. "
                "Only one container can be selected without configuring supported file types."
            ));
        }

        // Now we have one or more media image containers and an emulator selected,
        // let's fetch the media image container data.

        foreach(QModelIndex mind, listMIndex) {
            if (!mind.isValid()) continue;
            EmuFrontObject *obImg = dbMic->getDataObjectFromModel(&mind); // throws EmuFrontException
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

        if (mame) {
            emuHelper->launch(exe, mediaImageContainers);
            return;
        }
        else {
            // mediaImageContainers list contains all the selected media image containers and
            // mediaImages list contains all the media images inside all the selected containers

            QList<EmuFrontObject*> selectedImages;
            if (mediaImages.count() < 1) {
                throw EmuFrontException("No media images available!");
            }

            // check if command options have slots for nr media images > 1 e.g. "-diska $1 -diskb $2 ..."
            QString opts = exe->getOptions();
            QRegExp rx("(\\$\\d+)");
            QStringList list;
            int pos = 0;
            while ((pos = rx.indexIn(opts, pos)) != -1) {
                list << rx.cap(1);
                pos += rx.matchedLength();
            }
            bool ok;

            if (list.count() > mediaImages.count()) {
                throw EmuFrontException(tr("Select %1 media images for this emulator configuration").arg(list.count()));
            }
            if (list.count() > 1) {
                // more than one placeholder for media image in the command line ($1, $2, ...)
                int lim = list.count() == mediaImages.count() ? list.count() - 1 : list.count();
                // user sets the order of media images
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
                // there should be at least one media image left in mediaImages map...
                /*if (mediaImages.count() == 1) {
                selectedImages << mediaImages.values().first();
            } ... this is added later-> */
            }
            else if (mediaImages.count() > 1) {
                // show select boot image dialog
                EmuFrontObject *efo = EmuFrontInputDialog::getItem(
                        this, tr("Select boot image"), tr("Select"), mediaImages.values(), 0, false, &ok);
                if (!ok)  {
                    throw EmuFrontException(tr("Boot image selection was canceled, aborting."));
                }
                selectedImages << efo;
                MediaImage *mi = dynamic_cast<MediaImage*>(efo);
                QString key = mi->getCheckSum();
                mediaImages.remove(key);
            }
            else if (mediaImages.count() == 1) {
                EmuFrontObject *efo = mediaImages.values().first();
                selectedImages << efo;
                MediaImage *mi = dynamic_cast<MediaImage*>(efo);
                QString key = mi->getCheckSum();
                mediaImages.remove(key);
            }
            // in all the both cases the (ordered) list of media images will be passed to emuHelper

            // wee also keep the rest of the mediaimages in the selected containers for reference!
            foreach(EmuFrontObject *efo, mediaImages) {
                selectedImages << efo;
            }

            if (selectedImages.count() < 1)
                throw EmuFrontException(tr("No media images selected"));

            emuHelper->launch(exe, mediaImageContainers, selectedImages, list.count(), tmpDirPath);
        }
    } catch (EmuFrontException efe) {
        errorMessage->showMessage(efe.what());
    }

    micTable->clearSelection();
    if (exe) delete exe;
    qDeleteAll(mediaImageContainers);
    //qDeleteAll(mediaImages); these are already deleted along with containers
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

void EmuLauncher::setTmpDirPath(QString tmp)
{
    tmpDirPath = tmp;
}

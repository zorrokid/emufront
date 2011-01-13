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
#include <QProcess>
#include <QSqlTableModel>
#include <QItemSelectionModel>
#include "emulauncher.h"
#include "setup.h"
#include "setupmodel.h"
#include "externalexecutablemodel.h"
#include "mediaimagecontainer.h"
#include "mediaimagecontainermodel.h"
#include "executable.h"
#include "emuhelper.h"
#include "emufrontinputdialog.h"
#include "emufrontexception.h"

EmuLauncher::EmuLauncher(QErrorMessage *errorMessage, SetupModel *supModel, ExternalExecutableModel
		*emuModel, QWidget *parent, QString tmp) :
    QWidget(parent), errorMessage(errorMessage), supModel(supModel), emuModel(emuModel), tmpDirPath(tmp)
{
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
    qDebug() << "EmuLauncher::updateData";
    micModel->refresh();
    emuModel->refresh();
}

void EmuLauncher::initWidgets()
{
    micTable = new QTableView(this);
    micTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    micTable->setCornerButtonEnabled(false);
    micTable->verticalHeader()->setVisible(false);
    micTable->horizontalHeader()->setClickable(false);

    micModel = new MediaImageContainerModel(this);
    micTable->setModel(micModel);

    //supModel = new SetupModel(this);
    setupSelectBox = new QComboBox(this);
    setupSelectBox->setModel(supModel);
    setupSelectBox->setModelColumn(SetupModel::Setup_Name);

    //emuModel = new ExternalExecutableModel(this);
    execSelectBox = new QComboBox(this);
    execSelectBox->setModel(emuModel);
    execSelectBox->setModelColumn(ExternalExecutableModel::Executable_Name);

    selectButton = new QPushButton(tr("&Update"), this);
    launchButton = new QPushButton(tr("&Launch"), this);
}

void EmuLauncher::layout()
{
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(setupSelectBox, 0, 0, 1, 2);
    grid->addWidget(selectButton, 0, 2);
    grid->setColumnStretch(3, 1);

    grid->addWidget(micTable, 1, 0, 1, 4);
    grid->addWidget(execSelectBox, 2, 0);
    grid->addWidget(launchButton, 2, 1);
    // grid will be implicitly parented to this
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
    if (setupSelectBox->currentIndex() == -1) return;

    // 1. get selected platform and media type id
    //QAbstractItemModel *setupAbsModel = setupSelectBox->model();
    //SetupModel *supModel = qobject_cast<SetupModel *>(setupAbsModel);
    //if (!supModel) return;

    QModelIndex supInd =
        supModel->index(setupSelectBox->currentIndex(), SetupModel::Setup_Id);
    int supId = supModel->data(supInd).toInt();
    if (supId < 0) return;

    // 2. fetch available media image containers

    QAbstractItemModel *absModel = micTable->model();
    MediaImageContainerModel *micModel = qobject_cast<MediaImageContainerModel*>(absModel);
    micModel->filterBySetup(supId);

    micTable->hideColumn(MediaImageContainerModel::MIC_FileId);
    micTable->hideColumn(MediaImageContainerModel::MIC_FileSize);
    micTable->hideColumn(MediaImageContainerModel::MIC_FileCheckSum);
    micTable->hideColumn(MediaImageContainerModel::MIC_FilePathId);
    micTable->hideColumn(MediaImageContainerModel::MIC_FilePathName);
    micTable->hideColumn(MediaImageContainerModel::MIC_SetupId);
    micTable->hideColumn(MediaImageContainerModel::MIC_PlatformName);
    micTable->hideColumn(MediaImageContainerModel::MIC_PlatformId);
    micTable->hideColumn(MediaImageContainerModel::MIC_MediaTypeName);
    micTable->hideColumn(MediaImageContainerModel::MIC_MediaTypeId);
    micTable->resizeColumnsToContents();

    // 3. filter available emulators
    QAbstractItemModel *execAbsModel = execSelectBox->model();
    ExternalExecutableModel *execModel = qobject_cast<ExternalExecutableModel*>(execAbsModel);
    if (!execModel) return;
    execModel->filterBySetup(supId);
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

        QAbstractItemModel *absModel = execSelectBox->model();
        ExternalExecutableModel *extModel = qobject_cast<ExternalExecutableModel*>(absModel);
        exe = extModel->getExecutable(execSelectBox->currentIndex());
        if (!exe) {
            errorMessage->showMessage(tr("Failed creating an executable object from selection."));
            return;
        }

        bool mame = exe->getSetup()->getSupportedFileTypeExtensions().isEmpty();

        if (mame && listMIndex.count() > 1) {
            throw EmuFrontException(tr("No supported file types configured for this emulator configuration. "
                "Assuming emulator support container files as is. "
                "Only one container can be selected without configuring supported file types."
            ));
        }

        // Now we have one or more media image containers and an emulator selected,
        // let's fetch the media image container data.

        QAbstractItemModel *micAbsModel = micTable->model();
        MediaImageContainerModel *micModel = qobject_cast<MediaImageContainerModel *>(micAbsModel);
        if (!micModel) {
            throw new EmuFrontException(tr("Failed creating data model for media image containers."));
        }

        foreach(QModelIndex mind, listMIndex) {
            if (!mind.isValid()) continue;

            EmuFrontObject *obImg = micModel->getDataObject(mind);

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

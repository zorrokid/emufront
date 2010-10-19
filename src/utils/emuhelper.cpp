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

#include <QDebug>
#include <QMessageBox>
#include "emuhelper.h"
#include "unziphelper.h"
#include "../dataobjects/mediaimagecontainer.h"
#include "../dataobjects/executable.h"
#include "../exceptions/emufrontexception.h"

EmuHelper::EmuHelper(QObject *parent) :
    ProcessHelper(parent)
{
    unzipHelper = new UnzipHelper(this);
}

void EmuHelper::launch(const Executable * ex, QList<MediaImageContainer *> micList,
    QList<EmuFrontObject *> miList)
{
    if (miList.count() < 1) {
        throw EmuFrontException(tr("No media images available!"));
    }
    if (micList.count() < 1) {
        throw EmuFrontException(tr("No media image containers available!"));
    }
    // extract the media image container to tmp folder
    // (TODO: tmp folder configuration)

    foreach(MediaImageContainer *mic, micList) {
        QString fp;
        fp.append(mic->getFilePath()->getName());
        if (!fp.endsWith('/')) fp.append("/");
        fp.append(mic->getName());
        int ret = unzipHelper->extractAll(fp, "/tmp/");
        if (ret) {
            qDebug() << "Failed unzipping " << fp << ".";
            //throw EmuFrontException(tr("Unzip failed with %1.").arg(ret));
        }
    }

    // TODO: launch the 1st media image in the media image list of ex
    // or if emulator command options has a place for more than one
    // media image assign the media images in the list order
    // to emulator command line.

    QString opts = ex->getOptions();
    QString tmpfp = " \"/tmp/";
    // if only one media image placeholder -> TODO: if more placeholders e.g. $1 $2 ...
    tmpfp.append(miList.first()->getName()).append("\"");
    opts.replace("$1", tmpfp);
    QString cmdWithParams;
    cmdWithParams.append(ex->getExecutable());
    cmdWithParams.append(" ").append(opts);
    // TODO: tmp will be set dynamically
    // TODO: assigning multiple media images
    qDebug() << "Command with params " << cmdWithParams;
    // Executable and MediaImageContainer / MediaImage objects are no more needed:
    delete ex;
    qDeleteAll(micList);
    //qDeleteAll(miList); these objects are already deleted along with micList
    start(cmdWithParams, QIODevice::ReadOnly);
}

void EmuHelper::processError(QProcess::ProcessError)
{

}

void EmuHelper::processFinished(int)
{
}

void EmuHelper::connectSignals()
{
    connect(unzipHelper, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
    connect(unzipHelper, SIGNAL(finished(int)), this, SLOT(processFinished(int)));
}
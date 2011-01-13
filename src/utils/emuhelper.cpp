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
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include "emuhelper.h"
#include "unziphelper.h"
#include "mediaimagecontainer.h"
#include "executable.h"
#include "emufrontexception.h"

EmuHelper::EmuHelper(QObject *parent) :
    ProcessHelper(parent)
{
    unzipHelper = new UnzipHelper(this);
}


// TODO: These two launch functions may be merged to one and/or split into some common helper functions.
/* Throws EmuFrontException */
void EmuHelper::launch(const Executable * ex, QList<MediaImageContainer *> micList)
{
    if (micList.count() < 1) {
        throw EmuFrontException(tr("No media image containers available!"));
    }

    MediaImageContainer *mic = micList.first();
    QString fp = " \"";
    fp.append(mic->getFilePath()->getName());
    if (!fp.endsWith('/'))
        fp.append("/");
    fp.append(mic->getName());
    fp.append("\"");

    QString opts = ex->getOptions();
    opts.replace(QString("$1"), fp);

    QString cmdWithParams;
    cmdWithParams.append(ex->getExecutable());
    cmdWithParams.append(" ").append(opts);

    qDebug() << "Command with params " << cmdWithParams;
    start(cmdWithParams, QIODevice::ReadOnly);

    // for the moment, we'll wait for the process to be finished until we continue
    waitForFinished(-1);

    // these will be deleted in calling function:
    //delete ex;
    //qDeleteAll(micList);
}

/* Throws EmuFrontException */
void EmuHelper::launch(const Executable * ex, QList<MediaImageContainer *> micList,
    QList<EmuFrontObject *> miList, int mediaCount, QString tmp)
{
    if (miList.count() < 1) {
        throw EmuFrontException(tr("No media images available!"));
    }
    if (micList.count() < 1) {
        throw EmuFrontException(tr("No media image containers available!"));
    }

    if (!tmp.endsWith('/')) tmp.append("/");

    // extract the media image container to tmp folder
    foreach(MediaImageContainer *mic, micList) {
        QString fp;
        fp.append(mic->getFilePath()->getName());
        if (!fp.endsWith('/')) fp.append("/");
        fp.append(mic->getName());
        qDebug() << "Extracting " << fp;
        int ret = unzipHelper->extractAll(fp, tmp); // throws EmuFrontException
        if (ret) {
            qDebug() << "Failed unzipping " << fp << ".";
        }
    }

    // fill in the media image slots in the command line options ($1, $2, ...)
    QString opts = ex->getOptions();
    for(int i = 0; i < mediaCount && i < miList.size(); i++) {
        QString tmpfp = " \"";
        tmpfp.append(tmp);
        tmpfp.append (miList.at(i)->getName());
        tmpfp.append("\"");
        opts.replace(QString("$%1").arg(i+1), tmpfp);
    }

    QString cmdWithParams;
    cmdWithParams.append(ex->getExecutable());
    cmdWithParams.append(" ").append(opts);

    qDebug() << "Command with params " << cmdWithParams;
    start(cmdWithParams, QIODevice::ReadOnly);

    // for the moment, we'll wait for the process to be finished until we continue
    waitForFinished(-1);

    try {
        QDir ftmp(tmp);
        if (!ftmp.exists()) {
            throw EmuFrontException(tr("Trying to remove temporary files. "
                                       "Directory %s doesn't exist!").arg(tmp));
        }
        // clean the temp dir
        // TODO: if selected archive with multiple items, the files are not removed!
        foreach(EmuFrontObject *ob, miList) {
            if (!ftmp.exists(ob->getName())) {
                qDebug() << "File " << ob->getName() << " doesn't exist in " << tmp;
                continue;
            }
            QString fp = ftmp.filePath(ob->getName());
            QFile f(fp);
            if (!f.exists()) {
                qDebug() << "File " << fp << " doesn't exist!";
            }
            if (!f.remove()) {
                qDebug() << "Removing " << fp << " failed.";
            }
        }
    } catch (EmuFrontException e) {
        qDebug() << e.what();
        throw e;
    }
    // these will be deleted in calling function:
    //delete ex;
    //qDeleteAll(micList);delete ex;
    //qDeleteAll(miList); these objects are already deleted along with micList
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

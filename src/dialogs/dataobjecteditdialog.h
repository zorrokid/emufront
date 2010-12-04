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

#ifndef DATAOBJECTEDITDIALOG_H
#define DATAOBJECTEDITDIALOG_H

#include "emufrontdialog.h"
#include "emufrontobject.h"

class QDialogButtonBox;
class QComboBox;

class DataObjectEditDialog : public EmuFrontDialog
{
       Q_OBJECT

public:
    DataObjectEditDialog(QWidget *parent, EmuFrontObject *ob, Qt::Orientation orientation = Qt::Horizontal);
    virtual void setDataObject(EmuFrontObject *) = 0;
    virtual void updateData();

protected:
    virtual void connectSignals();
    EmuFrontObject *efObject;
    QDialogButtonBox *buttonBox;
    void setSelected(QComboBox*, const EmuFrontObject*, int idIndex);
    virtual void closeEvent(QCloseEvent *);
    virtual void clear() = 0;

signals:
    void updateRejected();
    void dataObjectUpdated();
    void test();
    void dialogClosed();

protected slots:
    virtual void acceptChanges() =0;
    virtual void rejectChanges();

private:
    Qt::Orientation orientation;

};

#endif // DATAOBJECTEDITDIALOG_H

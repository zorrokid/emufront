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

#ifndef NAMEDIALOG_H
#define NAMEDIALOG_H

#include "emufrontobject.h"
#include "dataobjecteditdialog.h"

class QLabel;
class QLineEdit;
class QPushButton;

class NameDialog : public DataObjectEditDialog
{
	Q_OBJECT

public:
    NameDialog(QWidget *parent = 0, EmuFrontObject * = 0);
    ~NameDialog();
    virtual void setDataObject(EmuFrontObject *) = 0;

protected slots:
    virtual void acceptChanges();
    void enableSaveButton(const QString &);

protected:
    virtual void setDataObject(QString name) =0;
    virtual void clear();
    QLabel *nameLabel;
	QLineEdit *nameEdit;
private:
	void connectSignals();
	void layout();
};

#endif

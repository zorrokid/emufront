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

#ifndef EMUFRONTDATADIALOG_H
#define EMUFRONTDATADIALOG_H

#include "emufrontdialog.h"
#include "emufrontquerymodel.h"

class QDialogButtonBox;
class QTableView;

class EmuFrontEditView : public EmuFrontDialog
{
    Q_OBJECT
public:
    EmuFrontEditView(QWidget *parent = 0);

private slots:
    void editButtonClicked();
    void addButtonClicked();
    void deleteButtonClicked();
    void listObjectClicked(const QModelIndex &);

protected:
    EmuFrontQueryModel *model;
    QList<int> hiddenColumns;
    QDialogButtonBox *buttonBox;
    QTableView *objectList;
    void postInit();

private:
    QPushButton *editButton;
    QPushButton *addButton;
    QPushButton *deleteButton;
    void layout();
    virtual void setHiddenColumns();
    void hideColumns();
    virtual void connectSignals();
    void setButtonsEnabled(bool);

};

#endif // EMUFRONTDATADIALOG_H

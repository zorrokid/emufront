// EmuFront
// Copyright 2010 Mikko Keinänen
//
// This file is part of EmuFront.
//
//
// EmuFront is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Foobar is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DBOBJECTDIALOG_H
#define DBOBJECTDIALOG_H

#include "emufrontdialog.h"
#include "dataobjecteditdialog.h"
#include "../db/databasemanager.h"
#include "../dataobjects/emufrontobject.h"

class QPushButton;
class QModelIndex;
class QDialogButtonBox;
class QTableView;

class DbObjectDialog : public EmuFrontDialog
{
    Q_OBJECT

public:
    DbObjectDialog(QWidget *parent = 0);
    ~DbObjectDialog();

protected slots:
	void editButtonClicked();
	void addButtonClicked();
	void deleteButtonClicked();
	void listObjectClicked(const QModelIndex &);
    void updateData();
    void updateList() const;
    private slots:

protected:
    // implementation specific, deletes current data object from memory
    virtual void deleteCurrentObject();
	virtual void addObject() =0;
    virtual bool deleteItem() = 0;
    virtual void insertDb(const EmuFrontObject*) const = 0;
    void updateDb(const EmuFrontObject*) const;
    void editObject();
    void connectSignals();
    void activateNameDialog();
    void initDataTable();
    bool confirmDelete(QString name, int numRefs);
    DataObjectEditDialog *nameDialog;
    DatabaseManager *dbManager;
    QTableView *objectList;
    EmuFrontObject *dbObject;

private:
	void setButtonsEnabled(bool);
    void layout();
    void disableSelection();
    QDialogButtonBox *buttonBox;
    QPushButton *editButton;
    QPushButton *addButton;
    QPushButton *deleteButton;

};

#endif

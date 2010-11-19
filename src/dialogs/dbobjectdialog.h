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
    virtual void refreshDataModel();

private slots:
	void editButtonClicked();
	void addButtonClicked();
	void deleteButtonClicked();
	void listObjectClicked(const QModelIndex &);
    void updateData();
    void updateReject();
    void updateList() const;
    void testSlot();
    void enableUi();

protected:
    // implementation specific, deletes current data object from memory
    virtual void deleteCurrentObject() = 0;
    virtual void cleanUp() = 0;
    virtual void initEditDialog() = 0;
    virtual EmuFrontObject* createObject() = 0;
    virtual void closeEvent(QCloseEvent *);
    void initDataTable();
    virtual void connectSignals();
    virtual void connectNameDialogSignals();
    DataObjectEditDialog *nameDialog;
    EmuFrontObject *dbObject;
    DatabaseManager *dbManager;
    QDialogButtonBox *buttonBox;
    QTableView *objectList;
    QList<int> hiddenColumns;
    void hideColumns();
    void setUIEnabled(bool);

private:
    bool deleteItem();
    void addObject();
    void insertDb(const EmuFrontObject*) const;
    void updateDb(const EmuFrontObject*) const;
    void editObject();
    void activateNameDialog(bool updateData = true);
    bool confirmDelete(QString name, int numRefs);
    void setButtonsEnabled(bool);
    void layout();
    void disableSelection();
    void createEditDialog();
    QPushButton *editButton;
    QPushButton *addButton;
    QPushButton *deleteButton;
};

#endif

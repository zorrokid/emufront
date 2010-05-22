// EmuFront
// Copyright Mikko Keinänen 2010
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
#include "namedialog.h"
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
	//void enableEditButton();
	//void enableDeleteButton();
	void listObjectClicked(const QModelIndex &);
    virtual void updateData();
    void updateList() const;

    protected:
	virtual int deleteObject() =0;
	virtual void addObject() =0;
	virtual void editObject() =0;
    virtual bool deleteItem() = 0;
    NameDialog *nameDialog;
    DatabaseManager *dbManager;
    QTableView *objectList;
    EmuFrontObject *dbObject;
    void connectSignals();
    void activateNameDialog() const;
    virtual void updateDb(const EmuFrontObject*) const = 0;
    virtual void insertDb(const EmuFrontObject*) const = 0;
    //virtual QSqlTableModel* getDataObjects() = 0;

    private:
	QDialogButtonBox *buttonBox;
	QPushButton *editButton;
    QPushButton *addButton;
	QPushButton *deleteButton;

	void setButtonsEnabled(bool);
    void layout();
    void disableSelection();
};

#endif

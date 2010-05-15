#ifndef DBOBJECTDIALOG_H
#define DBOBJECTDIALOG_H

#include "emufrontdialog.h"
#include "namedialog.h"
#include "../db/databasemanager.h"

class QPushButton;
class QModelIndex;
class QDialogButtonBox;
class QTableView;

class DbObjectDialog : public EmuFrontDialog
{
    Q_OBJECT

    public:
	DbObjectDialog(QWidget *parent = 0);

    protected slots:
	void editButtonClicked();
	void addButtonClicked();
	void deleteButtonClicked();
	//void enableEditButton();
	//void enableDeleteButton();
	void listObjectClicked(const QModelIndex &);
    void updateList() const;

    protected:
	virtual int deleteObject() =0;
	virtual void addObject() =0;
	virtual void editObject() =0;
    NameDialog *nameDialog;
    DatabaseManager *dbManager;
    QTableView *objectList;

    private:
	QDialogButtonBox *buttonBox;
	QPushButton *editButton;
	QPushButton *addButton;
	QPushButton *deleteButton;

	void setButtonsEnabled(bool);
	void connectSignals();
	void layout();
	void disableSelection();
};

#endif

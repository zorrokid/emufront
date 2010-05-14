#ifndef DBOBJECTDIALOG_H
#define DBOBJECTDIALOG_H

#include "namedialog.h"

class QPushButton;
class QModelIndex;
class QDialogButtonBox;
class QListView;

class DbObjectDialog : public QDialog 
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
	
    private:
	QDialogButtonBox *buttonBox;
	QPushButton *editButton;
	QPushButton *addButton;
	QPushButton *deleteButton;
	QListView *objectList;
	NameDialog *nameDialog;

	void setButtonsEnabled(bool);
	int deleteObject();
	void connectSignals();
	void layout();
	void disableSelection();
};

#endif

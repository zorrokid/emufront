#ifndef DBOBJECTDIALOG_H
#define DBOBJECTDIALOG_H

#include <QDialog>
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

    protected:
	virtual int deleteObject() =0;
	virtual void addObject() =0;
	virtual void editObject() =0;
        NameDialog *nameDialog;
	
    private:
	QDialogButtonBox *buttonBox;
	QPushButton *editButton;
	QPushButton *addButton;
	QPushButton *deleteButton;
	QListView *objectList;

	void setButtonsEnabled(bool);
	void connectSignals();
	void layout();
	void disableSelection();
};

#endif

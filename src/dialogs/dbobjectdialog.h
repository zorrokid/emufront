#ifndef DBOBJECTDIALOG_H
#define DBOBJECTDIALOG_H

#include <QDialog>

class QPushButton;
class QModelIndex;
class QDialogButtonBox;
class QListView;
class NameDialog;

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
	
    private:
	QDialogButtonBox *buttonBox;
	QPushButton *editButton;
	QPushButton *addButton;
	QPushButton *deleteButton;
	QListView *objectList;
	NameDialog *nameDialog;

	void setButtonsEnabled(bool);
	void connectSignals();
	void layout();
	void disableSelection();
};

#endif

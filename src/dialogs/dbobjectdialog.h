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

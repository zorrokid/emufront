#ifndef PLATFORMDIALOG_H
#define PLATFORMDIALOG_H

#include "dbobjectdialog.h"

class PlatformDialog : public DbObjectDialog
{
    Q_OBJECT

    public:
	PlatformDialog(QWidget *parent = 0);
    ~PlatformDialog();

    protected:
	virtual int deleteObject();
	virtual void addObject();
	virtual void editObject();
    virtual bool deleteItem();
    virtual void updateDb(const EmuFrontObject*) const;
    virtual void insertDb(const EmuFrontObject*) const;
    virtual QSqlTableModel* getDataObjects();

    private slots:
    virtual void updateData();

    private:
    enum {
        Platform_Id = 0,
        Platform_Name = 1,
        Platform_Filename = 2 };
        static const QString DB_TABLE_NAME_PLATFORM;
        //PlatformNameDialog *nameDialog;
};

#endif

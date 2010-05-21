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

    private slots:
    virtual void updateData();

    private:
        //PlatformNameDialog *nameDialog;
};

#endif

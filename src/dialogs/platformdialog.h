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

    private slots:
    virtual void updateData();
	
        //PlatformNameDialog *nameDialog;
};

#endif

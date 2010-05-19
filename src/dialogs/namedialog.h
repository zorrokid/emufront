#ifndef NAMEDIALOG_H
#define NAMEDIALOG_H

#include "../dataobjects/emufrontobject.h"
#include "emufrontdialog.h"


class QLabel;
class QLineEdit;
class QPushButton;
class QDialogButtonBox;

class NameDialog : public EmuFrontDialog
{
	Q_OBJECT

public:
    NameDialog(QWidget *parent = 0, EmuFrontObject * = 0);
    ~NameDialog();
    void setDataObject(EmuFrontObject *);

signals:
    void dataObjectUpdated();

protected slots:
    void acceptChanges();
	void enableSaveButton(const QString &);

protected:
    virtual void setDataObject(QString name) =0;
	QLabel *nameLabel;
	QLineEdit *nameEdit;
    QDialogButtonBox *buttonBox;
    EmuFrontObject *efObject;

private:
	void connectSignals();
	void layout();
};

#endif

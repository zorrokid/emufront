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

signals:
	void insertName(const QString &name, int id = 0);

protected slots:
	void saveButtonClicked();
	void enableSaveButton(const QString &);

protected:
    virtual void setDataObject(QString name) =0;
	QLabel *nameLabel;
	QLineEdit *nameEdit;
    QDialogButtonBox *buttonBox;
    //QPushButton *saveButton;
    //QPushButton *closeButton;
    EmuFrontObject *efObject;

private:
	void connectSignals();
	void layout();
    void close(bool);
};

#endif

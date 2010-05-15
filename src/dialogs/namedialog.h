#ifndef NAMEDIALOG_H
#define NAMEDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

class NameDialog : public QDialog
{
	Q_OBJECT

public:
	NameDialog(QWidget *parent = 0, bool edit = false);
	//~NameDialog();
	void setEdit(bool edit);

signals:
	void insertName(const QString &name, int id = 0);

protected slots:
	void saveButtonClicked();
	void enableSaveButton(const QString &);

protected:
	virtual void save() =0;
	QLabel *nameLabel;
	QLineEdit *nameEdit;
	QPushButton *saveButton;
	QPushButton *closeButton;
	bool edit;
private:
	void connectSignals();
	void layout();
};

#endif

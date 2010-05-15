#ifndef EMUFRONTDIALOG_H
#define EMUFRONTDIALOG_H
#include <QDialog>

class QErrorMessage;

class EmuFrontDialog : public QDialog
{
        Q_OBJECT
public:
        EmuFrontDialog(QWidget *parent);
protected:
        QErrorMessage *errorMessage;
signals:
        void dbUpdated();

};


#endif // EMUFRONTDIALOG_H

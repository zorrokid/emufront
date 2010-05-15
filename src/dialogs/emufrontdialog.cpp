
#include <QtGui>
#include "emufrontdialog.h"

EmuFrontDialog::EmuFrontDialog(QWidget *parent)
        : QDialog(parent)
{
    errorMessage = new QErrorMessage(this);
}

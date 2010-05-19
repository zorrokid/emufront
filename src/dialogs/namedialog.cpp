#include <QtGui>
#include "namedialog.h"

NameDialog::NameDialog(QWidget *parent, EmuFrontObject *efObj)
        : EmuFrontDialog(parent), efObject(efObj)
{
	nameLabel = new QLabel(tr("&Name: "));	
	nameEdit = new QLineEdit;
	nameLabel->setBuddy(nameEdit);
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Abort, Qt::Horizontal);
    /*saveButton = new QPushButton(tr("&Save"));
	saveButton->setDefault(true);
	saveButton->setEnabled(false);
    closeButton = new QPushButton(tr("Close"));*/
    connectSignals();
	layout();
	setWindowTitle(tr("Set names"));
}

NameDialog::~NameDialog()
{
    delete efObject;

	/* deleting objects in heap is not needed here
	 * because when deleting a parent widget
	 * the child widgets will be also deleted:
	 * delete nameLabel;
	 * delete nameEdit;
	 * delete saveButton;
	 * delete closeButton;
	 */
}

void NameDialog::connectSignals()
{
    connect(nameEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableSaveButton(const QString &)));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptChanges()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    /*connect(saveButton, SIGNAL(clicked()), this, SLOT(saveButtonClicked()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));*/
}

void NameDialog::layout()
{
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(nameLabel);
	topLayout->addWidget(nameEdit);

	QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(buttonBox);
    /*bottomLayout->addStretch();
	bottomLayout->addWidget(saveButton);
    bottomLayout->addWidget(closeButton);*/

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bottomLayout);
	setLayout(mainLayout);
}

void NameDialog::acceptChanges()
{
    if (nameEdit->text() == 0 || nameEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("Invalid input"), tr("Empty string is not accepted as name!"));
		return;
    }

	QString name = nameEdit->text().simplified();
    setDataObject(name);
    emit dataObjectUpdated();
    close();
}

void NameDialog::enableSaveButton(const QString &text)
{
    //saveButton->setEnabled(!text.isEmpty());
}



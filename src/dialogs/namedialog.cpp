#include <QtGui>
#include "namedialog.h"

NameDialog::NameDialog(QWidget *parent, bool edit)
	: QDialog(parent), edit(edit)
{
	nameLabel = new QLabel(tr("&Name: "));	
	nameEdit = new QLineEdit;
	nameLabel->setBuddy(nameEdit);
	saveButton = new QPushButton(tr("&Save"));
	saveButton->setDefault(true);
	saveButton->setEnabled(false);
	closeButton = new QPushButton(tr("Close"));
	connectSignals();
	layout();
	setWindowTitle(tr("Set names"));
}

NameDialog::~NameDialog()
{
	/* deleting objects in heap is not needed here
	 * because when deleting a parent widget
	 * the child widgets will be also deleted:
	 * delete nameLabel;
	 * delete nameEdit;
	 * delete saveButton;
	 * delete closeButton;
	 */
}

void NameDialog::setEdit(bool edit)
{
	this->edit = edit;
}
void NameDialog::connectSignals()
{
	connect(nameEdit, SIGNAL(textChanged(const QString &)), 
			this, SLOT(enableSaveButton(const QString &)));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveButtonClicked()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

void NameDialog::layout()
{
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(nameLabel);
	topLayout->addWidget(nameEdit);

	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
	bottomLayout->addWidget(saveButton);
	bottomLayout->addWidget(closeButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bottomLayout);
	setLayout(mainLayout);
}

void NameDialog::saveButtonClicked()
{
	if (nameEdit->text() == 0 || nameEdit->text().trimmed().isEmpty())
		return;

	QString name = nameEdit->text().simplified();
	/*if (edit) updateDb(name);
	else insertDb(name);*/
}

void NameDialog::enableSaveButton(const QString &text)
{
	saveButton->setEnabled(!text.isEmpty());
}

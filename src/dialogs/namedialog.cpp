// EmuFront
// Copyright 2010 Mikko Keinänen
//
// This file is part of EmuFront.
//
//
// EmuFront is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Foobar is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#include <QtGui>
#include "namedialog.h"

NameDialog::NameDialog(QWidget *parent, EmuFrontObject *efObj)
        : DataObjectEditDialog(parent, efObj)
{
	nameLabel = new QLabel(tr("&Name: "));	
	nameEdit = new QLineEdit;
	nameLabel->setBuddy(nameEdit);
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Abort, Qt::Horizontal);
    connectSignals();
	layout();
	setWindowTitle(tr("Set names"));
}

NameDialog::~NameDialog()
{
    // should be deleted in implementing classes
    // delete efObject;

    /* no need to delete parented QT-objects in heap here
	 * because when deleting a parent widget
     * the child widgets will be also deleted
	 */
}

void NameDialog::connectSignals()
{
    connect(nameEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableSaveButton(const QString &)));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptChanges()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(rejectChanges()));
}

void NameDialog::layout()
{
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(nameLabel);
	topLayout->addWidget(nameEdit);

	QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(buttonBox);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bottomLayout);
	setLayout(mainLayout);
}

void NameDialog::rejectChanges()
{
    efObject = 0;
    close();
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
    efObject = 0; // TODO we should also set efObject to null when user clicks abort
    close();
}

void NameDialog::enableSaveButton(const QString &text)
{
    //saveButton->setEnabled(!text.isEmpty());
}

void NameDialog::setDataObject(EmuFrontObject *ob)
{
    if (!ob) return;
    // delete efObject; -> we should not delete the previously referenced data object here, it may be still used in the parent widget
    // the parent widget will take of destruction
    // we'll just refresh the name dialog pointer to a new object
    efObject = ob;
}

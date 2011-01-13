/*
** EmuFront
** Copyright 2010-2011 Mikko Keinänen
**
** This file is part of EmuFront.
**
**
** EmuFront is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License version 2 as published by
** the Free Software Foundation and appearing in the file gpl.txt included in the
** packaging of this file.
**
** EmuFront is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QtGui>
#include "namedialog.h"

NameDialog::NameDialog(QWidget *parent, EmuFrontObject *efObj)
        : DataObjectEditDialog(parent, efObj)
{
	nameLabel = new QLabel(tr("&Name: "));	
    nameEdit = new QLineEdit;
    nameLabel->setBuddy(nameEdit);
    connectSignals();
	layout();
    emit test();
    setWindowTitle(tr("Set names"));
    setFocusProxy(nameEdit);
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
    DataObjectEditDialog::connectSignals();
    connect(nameEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableSaveButton(const QString &)));
}

void NameDialog::layout()
{
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(nameLabel);
	topLayout->addWidget(nameEdit);

	QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(buttonBox);
    buttonBox->setFocusPolicy(Qt::NoFocus);

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
    qDebug() << "We have a name " << name << ".";
    if (name != efObject->getName()) {
        setDataObject(name);
        emit dataObjectUpdated();
    }
    qDebug() << "Signal emitted.";
    efObject = 0; // TODO we should also set efObject to null when user clicks abort
    close();
}

void NameDialog::enableSaveButton(const QString &/*text*/)
{
    //saveButton->setEnabled(!text.isEmpty());
}

void NameDialog::setDataObject(EmuFrontObject *ob)
{
    if (!ob) return;
    // delete efObject; -> we should not delete the previously referenced data object here, it
    // may be still used in the parent widget
    // the parent widget will take care of destruction
    // we'll just refresh the name dialog pointer to a new object
    efObject = ob;
}

void NameDialog::clear()
{
    nameEdit->clear();
}

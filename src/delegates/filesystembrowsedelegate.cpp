// EmuFront
// Copyright 2010 Mikko Keinänen
//
// This file is part of EmuFront.
//
//
// EmuFront is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation and appearing in the file gpl.txt included in the
// packaging of this file.
//
// EmuFront is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EmuFront.  If not, see <http://www.gnu.org/licenses/>.

#include "filesystembrowsedelegate.h"
#include "filepathselectorwidget.h"

FileSystemBrowseDelegate::FileSystemBrowseDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* FileSystemBrowseDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    FilePathSelectorWidget *editor = new FilePathSelectorWidget(parent);
    connect(editor, SIGNAL(filePathUpdated()), this, SLOT(commitAndCloseEditor()));
    return editor;
}

void FileSystemBrowseDelegate::commitAndCloseEditor()
{
    FilePathSelectorWidget *editor = qobject_cast<FilePathSelectorWidget *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

void FileSystemBrowseDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    FilePathSelectorWidget *wdg = qobject_cast<FilePathSelectorWidget *>(editor);
    QString path = wdg->getFilePath();
    model->setData(index, path);
}

void FileSystemBrowseDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString path = index.model()->data(index, Qt::DisplayRole).toString();
    FilePathSelectorWidget *fpwdg = qobject_cast<FilePathSelectorWidget *>(editor);
    fpwdg->setFilePath(path);
}

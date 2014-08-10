// vim: set filetype=cpp:expandtab:tabstop=2
//  Copyright 2014 Cory Perry
//  This file is part of QSftpFileEngine.
//
//  QSftpFileEngine is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  QSftpFileEngine is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with QSftpFileEngine.  If not, see <http://www.gnu.org/licenses/>.


#include <QFileSystemModel>
#include <QCompleter>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include "filebrowser.h"
#include "ui_filebrowser.h"

FileBrowser::FileBrowser(const QString& path, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileBrowser),
    model(new QFileSystemModel(this))
{
    ui->setupUi(this);
    ui->treeView->setModel(model);
    ui->lineEdit->setCompleter(new QCompleter(model));
    QString p = (path.isEmpty() ? QDir::homePath() : path);
    ui->lineEdit->setText(p);
    model->setRootPath(ui->lineEdit->text());
    //for(int i=1;i<model->columnCount();i++)
    //    ui->treeView->hideColumn(i);
    on_lineEdit_returnPressed();
}

FileBrowser::~FileBrowser()
{
    delete ui;
}

void FileBrowser::on_lineEdit_returnPressed() {
    QFileInfo fi(ui->lineEdit->text());
    QModelIndex idx = model->index(fi.filePath());
    if(fi.exists() && idx.isValid()) {
        ui->treeView->setRootIndex(idx);
    }
    else
        QMessageBox::critical(this, tr("Path not found"), tr("Error indexing path '%1'").arg(fi.filePath()));
}

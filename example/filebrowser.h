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

#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QWidget>

namespace Ui {
class FileBrowser;
}

class QFileSystemModel;

class FileBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit FileBrowser(const QString& path = QString(), QWidget *parent = 0);
    ~FileBrowser();

private slots:
    void on_lineEdit_returnPressed();

private:
    Ui::FileBrowser *ui;
    QFileSystemModel* model;
};

#endif // FILEBROWSER_H

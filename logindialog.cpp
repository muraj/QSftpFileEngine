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

#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(const QString& _title, const QString _msg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->msgLabel->setText(_msg);
    if(_msg.isEmpty())
        ui->msgLabel->hide();
    this->setWindowTitle(_title);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

bool LoginDialog::login(QWidget* parent, QString& user, QString& passwd, const QString& title, const QString& msg) {
    LoginDialog dialog(title, msg, parent);
    if(dialog.exec() == QDialog::Accepted) {
        user = dialog.ui->userLineEdit->text();
        passwd = dialog.ui->passLineEdit->text();
        return true;
    }
    return false;
}

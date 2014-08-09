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

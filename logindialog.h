#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(const QString &title, const QString msg, QWidget *parent = 0);
    ~LoginDialog();
    static bool login(QWidget* parent, QString& user, QString& passwd, const QString& title = QString(), const QString& msg = QString());

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H

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

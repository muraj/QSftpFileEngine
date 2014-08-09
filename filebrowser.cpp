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
    for(int i=1;i<model->columnCount();i++)
        ui->treeView->hideColumn(i);
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

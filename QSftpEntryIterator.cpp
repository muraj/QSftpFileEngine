#include "QSftpEntryIterator.h"
#include "QSftpSession.h"
#include <QDebug>
#include <string>
#include <libssh/sftp.h>

QSftpEntryIterator::QSftpEntryIterator(QSharedPointer<QSftpSession> ssh, const QString& path, QDir::Filters filters, const QStringList& nameFilters) 
  : QAbstractFileEngineIterator(filters, nameFilters), session(ssh), attrs(NULL) {
  std::string path_str(path.toStdString());
  dir = sftp_opendir(session->sftpSession(), path_str.c_str());
}

QSftpEntryIterator::~QSftpEntryIterator() {
  if(attrs) {
    sftp_attributes_free(attrs);
    attrs = NULL;
  }
  if(dir) {
    sftp_closedir(dir);
    dir = NULL;
  }
}

bool QSftpEntryIterator::hasNext() const {
  return dir && sftp_dir_eof(dir) == 0;
}

QString QSftpEntryIterator::next() {
  if(!hasNext()) return QString();
  if(attrs) {
    sftp_attributes_free(attrs);
    attrs = NULL;
  }
  attrs = sftp_readdir(session->sftpSession(), dir);
  qDebug() << dir->name << '/' << currentFileName();
  return currentFileName();
}

QString QSftpEntryIterator::currentFileName() const {
  return (attrs ? QString(attrs->name) : QString());
}

#if 0
// TODO: psuedo-code to generate the QFileInfo from the attribute struct
QFileInfo QSftpEntryIterator::currentFileInfo() const {
  QAbstractFileEngine::FileFlags flags = QSftpFileEngine::fileFlags(attrs);
  return QFileInfo(flags);
}
#endif

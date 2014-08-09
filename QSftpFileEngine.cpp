#include "QSftpFileEngine.h"
#include "QSftpSession.h"
#include "QSftpEntryIterator.h"
#include <QDateTime>
#include <libssh/sftp.h>

QMap<QString, QSharedPointer<QSftpSession> > QSftpFileEngine::sessions;

QSftpFileEngine::QSftpFileEngine(const QSharedPointer<QSftpSession>& _host, const QString& fileName) : QAbstractFileEngine(), host(host), path(fileName.toStdString()) {
  if(host->errorCode() != SSH_NO_ERROR)
    setError(QFile::ResourceError, host->getSshError());
  if(!host->startSftp())
    setError(QFile::ResourceError, host->getSftpError());
}

QSftpFileEngine::~QSftpFileEngine() {

}

QStringList QSftpFileEngine::entryList(QDir::Filters, const QStringList& fileName) const {
  return QStringList();
}


QAbstractFileEngine::Iterator* QSftpFileEngine::beginEntryList(QDir::Filters filters,
    const QStringList& filterNames) {
  return new QSftpEntryIterator(host, QString::fromStdString(path), filters, filterNames);
}

QAbstractFileEngine::FileFlags QSftpFileEngine::fileFlags(
    QAbstractFileEngine::FileFlags flags) const {
  sftp_attributes attrs = sftp_lstat(host->sftpSession(), path.c_str());
  flags = fileFlags(attrs, flags);
  sftp_attributes_free(attrs);
  return flags;
}

QString QSftpFileEngine::fileName(QAbstractFileEngine::FileName file) const {
  sftp_attributes attrs = sftp_lstat(host->sftpSession(), path.c_str());
  QString ret;
  switch(file) {
  case QAbstractFileEngine::DefaultName:
  case QAbstractFileEngine::BaseName:
  case QAbstractFileEngine::PathName:
  case QAbstractFileEngine::AbsoluteName:
  case QAbstractFileEngine::AbsolutePathName:
  case QAbstractFileEngine::LinkName:
  case QAbstractFileEngine::CanonicalName:
  case QAbstractFileEngine::CanonicalPathName:
  case QAbstractFileEngine::BundleName: break;
  }
  sftp_attributes_free(attrs);
  return ret;
}

QDateTime QSftpFileEngine::fileTime(QAbstractFileEngine::FileTime time) const {
  switch(time) {
  case QAbstractFileEngine::CreationTime:
  case QAbstractFileEngine::ModificationTime:
  case QAbstractFileEngine::AccessTime:
    break;
  }
  return QDateTime();
}

bool QSftpFileEngine::supportsExtension(QAbstractFileEngine::Extension extension) const {
  return false;
}

bool QSftpFileEngine::isRelativePath() const {
  return false;
}

QSharedPointer<QSftpSession> QSftpFileEngine::session(const QString& user, const QString& host) {
  QString key = user + '@' + host;
  QMap<QString, QSharedPointer<QSftpSession> >::iterator i = sessions.find(key);
  if(i != sessions.end()) return i.value();
  QSharedPointer<QSftpSession> ret(new QSftpSession(user, host));
  sessions[key] = ret;
  return ret;
}

void QSftpFileEngine::closeSession(const QString& host) {
  sessions.remove(host);
}

void QSftpFileEngine::parseFileName(const QString& fileName, QString& user, QString& host, QString& path) {
  path = fileName.section(':', 1);
  if(path.isEmpty()) {
    path = fileName;
    return;
  }
  QString host_str(fileName.section(':', 0, 0));
  host = host_str.section('@', 1);
  if(host.isEmpty())
    host = host_str;
  else
    user = host_str.section('@', 0, 0);
}

QAbstractFileEngine::FileFlags QSftpFileEngine::fileFlags(sftp_attributes_struct* attrs, QAbstractFileEngine::FileFlags flags) {
  return QAbstractFileEngine::FileFlags();
}

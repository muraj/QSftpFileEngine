#include "QSftpFileEngine.h"
#include "QSftpSession.h"
#include <QAbstractFileEngineIterator>
#include <QDateTime>
#include <libssh/libssh.h>

QMap<QString, QSharedPointer<QSftpSession> > QSftpFileEngine::sessions;

QSftpFileEngine::QSftpFileEngine(const QSharedPointer<QSftpSession>& host, const QString& fileName) : QAbstractFileEngine() {
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
  return NULL;
}

QAbstractFileEngine::FileFlags QSftpFileEngine::fileFlags(
    QAbstractFileEngine::FileFlags flags) const {
  return QAbstractFileEngine::FileFlags();
}

QString QSftpFileEngine::fileName(QAbstractFileEngine::FileName file) const {
  return QString();
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


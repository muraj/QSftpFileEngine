#include "QSftpFileEngine.h"
#include "QSftpSession.h"
#include <QAbstractFileEngineIterator>
#include <QDateTime>

QMap<QString, QSharedPointer<QSftpSession> > QSftpFileEngine::sessions;

QSftpFileEngine::QSftpFileEngine(const QSharedPointer<QSftpSession>& host, const QString& fileName) : QAbstractFileEngine() {

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

QSharedPointer<QSftpSession> QSftpFileEngine::session(const QString& host) {
  QMap<QString, QSharedPointer<QSftpSession> >::iterator i = sessions.find(host);
  if(i != sessions.end()) return i.value();
  QSharedPointer<QSftpSession> ret(new QSftpSession(host));
  sessions[host] = ret;
  return ret;
}

void QSftpFileEngine::closeSession(const QString& host) {
  sessions.remove(host);
}

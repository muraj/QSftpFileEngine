#include "QSftpFileEngine.h"
#include <QAbstractFileEngineIterator>
#include <QDateTime>

QSftpFileEngine::QSftpFileEngine(const QString& host, const QString& fileName) : QAbstractFileEngine() {

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
  return QDateTime();
}

bool QSftpFileEngine::supportsExtension(QAbstractFileEngine::Extension extension) const {
  return false;
}

bool QSftpFileEngine::isRelativePath() const {
  return false;
}

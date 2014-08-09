#include "QSftpEntryIterator.h"

QSftpEntryIterator::QSftpEntryIterator(QSharedPointer<QSftpSession> ssh, QDir::Filters filters, const QStringList& nameFilter) 
  : QAbstractFileEngineIterator(filters, nameFilter), session(ssh) {

}

QSftpEntryIterator::~QSftpEntryIterator() {

}

bool QSftpEntryIterator::hasNext() const {
  return false;
}

QString QSftpEntryIterator::next() {
  return QString();
}

QString QSftpEntryIterator::currentFileName() const {
  return QString();
}

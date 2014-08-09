#include "QSftpEntryIterator.h"

QSftpEntryIterator::QSftpEntryIterator(QDir::Filters filters, const QStringList& nameFilter) 
  : QAbstractFileEngineIterator(filters, nameFilter) {

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

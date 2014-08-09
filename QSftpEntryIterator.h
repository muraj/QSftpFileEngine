#ifndef QSFTPENTRYITERATOR_H
#define QSFTPENTRYITERATOR_H

#include <QAbstractFileEngineIterator>

class QSftpEntryIterator : public QAbstractFileEngineIterator {
public:
  QSftpEntryIterator(QDir::Filters filters, const QStringList& nameFilter);
  virtual ~QSftpEntryIterator();
  bool hasNext() const;
  QString next();
  QString currentFileName() const;
};
#endif // QSFTPENTRYITERATOR_H

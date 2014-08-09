#ifndef QSFTPENTRYITERATOR_H
#define QSFTPENTRYITERATOR_H

#include <QAbstractFileEngineIterator>
#include <QSharedPointer>

class QSftpSession;

class QSftpEntryIterator : public QAbstractFileEngineIterator {
public:
  QSftpEntryIterator(QSharedPointer<QSftpSession> ssh, QDir::Filters filters, const QStringList& nameFilter);
  virtual ~QSftpEntryIterator();
  bool hasNext() const;
  QString next();
  QString currentFileName() const;
private:
  QSharedPointer<QSftpSession> session;
};
#endif // QSFTPENTRYITERATOR_H

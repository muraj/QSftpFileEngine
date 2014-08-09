#ifndef QSFTPENTRYITERATOR_H
#define QSFTPENTRYITERATOR_H

#include <QAbstractFileEngineIterator>
#include <QSharedPointer>

class QSftpSession;
class sftp_dir_struct;
class sftp_attributes_struct;

class QSftpEntryIterator : public QAbstractFileEngineIterator {
public:
  QSftpEntryIterator(QSharedPointer<QSftpSession> ssh, const QString& path, QDir::Filters filters, const QStringList& nameFilter);
  virtual ~QSftpEntryIterator();
  bool hasNext() const;
  QString next();
  QString currentFileName() const;
  // Since we have the attributes struct already, we should be able to build a QFileInfo off of it
  //QFileInfo currentFileInfo() const;
private:
  QSharedPointer<QSftpSession> session;
  sftp_dir_struct* dir;
  sftp_attributes_struct* attrs;
};
#endif // QSFTPENTRYITERATOR_H

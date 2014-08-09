#ifndef QSFTPFILEENGINE_H
#define QSFTPFILEENGINE_H

#include <QAbstractFileEngine>
#include <QMap>
#include <QSharedPointer>

class QSftpSession;
struct sftp_attributes_struct;

class QSftpFileEngine : public QAbstractFileEngine {
public:
  QSftpFileEngine(const QSharedPointer<QSftpSession>& _ssh, const QString& fileName);
  virtual ~QSftpFileEngine();
  bool caseSensitive() const { return true; }
  QStringList entryList(QDir::Filters, const QStringList& fileName) const;
  QAbstractFileEngine::Iterator* beginEntryList(QDir::Filters filters,
      const QStringList& filterNames);
  //bool close();
  //bool copy(const QString& copyName);
  //bool extension(QAbstractFileEngine::Extension ext
  //    const QAbstractFileEngine::ExtensionOption* option = 0,
  //    const QAbstractFileEngine::ExtensionReturn* output = 0);
  QAbstractFileEngine::FileFlags fileFlags(
      QAbstractFileEngine::FileFlags flags) const;
  QString fileName(QAbstractFileEngine::FileName file) const;
  QDateTime fileTime(QAbstractFileEngine::FileTime time) const;
  //bool flush();
  //int handle() const;
  bool isRelativePath() const;
  //bool isSequential() const;
  //bool link(const QString& newName);
  //bool mkdir(const QString& name, bool createParentDirectories) const;
  //bool open(QIODevice::OpenMode mode);
  //QString owner(QAbstractFileEngine::FileOwner own) const;
  //uint ownerId(QAbstractFileEngine::FileOwner own) const;
  //qint64 pos() const;
  //qint64 read(char* data, qint64 maxlen);
  //bool remove();
  //bool rename(const QString& newName);
  //bool rmdir(const QString& name, bool recurseParentDirectories) const;
  //bool seek(qint64 pos);
  //void setFileName(const QString& file);
  //bool setPermissions(uint perms);
  //bool setSize(qint64 size);
  //qint64 size() const;
  bool supportsExtension(QAbstractFileEngine::Extension extension) const;
  //qint64 write(const char* data, qint64 len);

  static QSharedPointer<QSftpSession> session(const QString& user, const QString& host);
  static void closeSession(const QString& host);
  static void parseFileName(const QString& fileName, QString& user, QString& host, QString& path);
  static QAbstractFileEngine::FileFlags fileFlags(sftp_attributes_struct* attrs, QAbstractFileEngine::FileFlags flags = QAbstractFileEngine::FileInfoAll);

private:
  QSharedPointer<QSftpSession> host;
  std::string path;
  static QMap<QString, QSharedPointer<QSftpSession> > sessions;
};

#endif // QSFTPFILEENGINE_H

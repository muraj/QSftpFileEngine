// vim: set filetype=cpp:expandtab:tabstop=2
//  Copyright 2014 Cory Perry
//  This file is part of QSftpFileEngine.
//
//  QSftpFileEngine is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  QSftpFileEngine is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with QSftpFileEngine.  If not, see <http://www.gnu.org/licenses/>.

#ifndef QSFTPFILEENGINE_H
#define QSFTPFILEENGINE_H

#include <QAbstractFileEngine>
#include <QMap>
#include <QSharedPointer>

class QSftpSession;
struct sftp_attributes_struct;
struct sftp_file_struct;

class QSftpFileEngine : public QAbstractFileEngine {
public:
  QSftpFileEngine(const QSharedPointer<QSftpSession>& _ssh, const QString& hostString, const QString& fileName);
  virtual ~QSftpFileEngine();
  bool caseSensitive() const { return true; }
  QStringList entryList(QDir::Filters, const QStringList& fileName) const;
  QAbstractFileEngine::Iterator* beginEntryList(QDir::Filters filters,
      const QStringList& filterNames);
  bool close();
  //bool copy(const QString& copyName);
  bool extension(QAbstractFileEngine::Extension ext,
      const QAbstractFileEngine::ExtensionOption* option = 0,
      const QAbstractFileEngine::ExtensionReturn* output = 0) { return false; }
  QAbstractFileEngine::FileFlags fileFlags(
      QAbstractFileEngine::FileFlags flags) const;
  QString fileName(QAbstractFileEngine::FileName file) const;
  QDateTime fileTime(QAbstractFileEngine::FileTime time) const;
  bool flush() { return true; }
  //int handle() const;
  bool isRelativePath() const;
  bool isSequential() const { return true; }
  //bool link(const QString& newName);
  //bool mkdir(const QString& name, bool createParentDirectories) const;
  bool open(QIODevice::OpenMode mode);
  QString owner(QAbstractFileEngine::FileOwner own) const;
  uint ownerId(QAbstractFileEngine::FileOwner own) const;
  qint64 pos() const;
  qint64 read(char* data, qint64 maxlen);
  bool remove();
  bool rename(const QString& newName);
  //bool rmdir(const QString& name, bool recurseParentDirectories) const;
  bool seek(qint64 pos);
  //void setFileName(const QString& file);
  //bool setPermissions(uint perms);
  bool setSize(qint64 size) { return false; }
  qint64 size() const;
  bool supportsExtension(QAbstractFileEngine::Extension extension) const { return false; }
  qint64 write(const char* data, qint64 len);

  static QSharedPointer<QSftpSession> session(const QString& user, const QString& host);
  static void closeSession(const QString& host);
  static void parseFileName(const QString& fileName, QString& user, QString& host, QString& path);
  static QAbstractFileEngine::FileFlags fileFlags(sftp_attributes_struct* attrs, const QString& baseName, QAbstractFileEngine::FileFlags flags = QAbstractFileEngine::FileInfoAll);

private:
  sftp_attributes_struct* getAttrs(bool refresh=false) const;
  sftp_file_struct* file;
  QSharedPointer<QSftpSession> host;
  QString hostString;
  QString path;
  mutable sftp_attributes_struct* attrs;
  static QMap<QString, QSharedPointer<QSftpSession> > sessions;
};

#endif // QSFTPFILEENGINE_H

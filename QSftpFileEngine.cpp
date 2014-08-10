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

#include "QSftpFileEngine.h"
#include "QSftpSession.h"
#include "QSftpEntryIterator.h"

#include <QDateTime>
#include <QMutex>
#include <QDebug>

#include <fcntl.h>

#include <libssh/sftp.h>

QMap<QString, QSharedPointer<QSftpSession> > QSftpFileEngine::sessions;

QSftpFileEngine::QSftpFileEngine(const QSharedPointer<QSftpSession>& _host, const QString& _hostString, const QString& fileName) 
    : QAbstractFileEngine(), host(_host), hostString(_hostString), path(fileName), attrs(NULL), file(NULL) {
  if(host->errorCode() != SSH_NO_ERROR)
    setError(QFile::ResourceError, host->getSshError());
  if(!host->startSftp())
    setError(QFile::ResourceError, host->getSftpError());
}

QSftpFileEngine::~QSftpFileEngine() {

}

QStringList QSftpFileEngine::entryList(QDir::Filters filters, const QStringList& nameFilters) const {
  QStringList ret;
  QAbstractFileEngine::Iterator* it
      = new QSftpEntryIterator(host, path, filters, nameFilters);
  while(it->hasNext())
    ret << it->next();
  delete it;
  return ret;
}


QAbstractFileEngine::Iterator* QSftpFileEngine::beginEntryList(QDir::Filters filters,
    const QStringList& filterNames) {
  return new QSftpEntryIterator(host, path, filters, filterNames);
}

QAbstractFileEngine::FileFlags QSftpFileEngine::fileFlags(
    QAbstractFileEngine::FileFlags flags) const {
  return fileFlags(getAttrs(flags.testFlag(QAbstractFileEngine::Refresh)),
      fileName(QAbstractFileEngine::BaseName), flags);
}

QString QSftpFileEngine::fileName(QAbstractFileEngine::FileName fileflags) const {
  QString ret;
  switch(fileflags) {
  case QAbstractFileEngine::DefaultName:
    ret = path; break;
  case QAbstractFileEngine::BaseName:
    ret = path.section('/', -1); break;
  case QAbstractFileEngine::PathName:
    ret = path.section('/', 0, -2); break;
  case QAbstractFileEngine::LinkName:
  {
    std::string p(path.toStdString());
    QMutexLocker locker(host->mutex());
    ret = QString(sftp_readlink(host->sftpSession(), p.c_str()));
    break;
  }
  case QAbstractFileEngine::BundleName: break;
  default:
  {
    std::string p(path.toStdString());
    QMutexLocker locker(host->mutex());
    QString absPath(sftp_canonicalize_path(host->sftpSession(), p.c_str()));
    switch(fileflags) {
    case QAbstractFileEngine::AbsoluteName:
    case QAbstractFileEngine::CanonicalName:
      ret = absPath;
      break;
    case QAbstractFileEngine::AbsolutePathName:
    case QAbstractFileEngine::CanonicalPathName:
      ret = absPath.section('/', 0, -2);
      break;
    }
    break;
  }
  }
  switch(fileflags) {
  case QAbstractFileEngine::DefaultName:
  case QAbstractFileEngine::BaseName:
  case QAbstractFileEngine::PathName: break;
  default:
    ret = hostString + ':' + ret; break;
  }
  qDebug() << "fileName(" << fileflags << ") = " << ret;
  return ret;
}

QDateTime QSftpFileEngine::fileTime(QAbstractFileEngine::FileTime time) const {
  sftp_attributes attrs = getAttrs();
  uint64_t unixtime;
  switch(time) {
  case QAbstractFileEngine::CreationTime:
    unixtime = attrs->createtime; break;
  case QAbstractFileEngine::ModificationTime:
    unixtime = attrs->mtime64; break;
  case QAbstractFileEngine::AccessTime:
    unixtime = attrs->atime64; break;
  }
  sftp_attributes_free(attrs);
  return QDateTime::fromMSecsSinceEpoch(unixtime);
}

bool QSftpFileEngine::open(QIODevice::OpenMode mode) {
  std::string p(path.toStdString());
  int accessType;
  if(mode.testFlag(QIODevice::ReadOnly))
    accessType |= O_RDONLY;
  if(mode.testFlag(QIODevice::WriteOnly))
    accessType |= O_WRONLY;
  if(mode.testFlag(QIODevice::ReadWrite))
    accessType |= O_RDWR;
  if(mode.testFlag(QIODevice::Truncate))
    accessType |= O_TRUNC;
  QMutexLocker locker(host->mutex());
  file = sftp_open(host->sftpSession(), p.c_str(), accessType, 0);
  if(file == NULL)
    setError(QFile::ResourceError, host->getSftpError());
  return file != NULL;
}
bool QSftpFileEngine::close() {
  QMutexLocker locker(host->mutex());
  int ret = sftp_close(file);
  if(ret == SSH_NO_ERROR)
    file = NULL;
  else
    setError(QFile::ResourceError, host->getSftpError());
  return ret == SSH_NO_ERROR;
}

QString QSftpFileEngine::owner(QAbstractFileEngine::FileOwner own) const {
  sftp_attributes attrs = getAttrs();
  QString ret;
  switch(own) {
  case QAbstractFileEngine::OwnerUser:
    ret = QString(attrs->owner); break;
  case QAbstractFileEngine::OwnerGroup:
    ret = QString(attrs->group); break;
  }
  return ret;
}

uint QSftpFileEngine::ownerId(QAbstractFileEngine::FileOwner own) const {
  sftp_attributes attrs = getAttrs();
  uint ret;
  switch(own) {
  case QAbstractFileEngine::OwnerUser:
    ret = attrs->uid; break;
  case QAbstractFileEngine::OwnerGroup:
    ret = attrs->gid; break;
  }
  return ret;
}

qint64 QSftpFileEngine::pos() const {
  QMutexLocker locker(host->mutex());
  return sftp_tell64(file);
}

qint64 QSftpFileEngine::read(char* data, qint64 maxlen) {
  QMutexLocker locker(host->mutex());
  int ret = sftp_read(file, data, maxlen);
  if(ret < 0)
    setError(QFile::ResourceError, host->getSftpError());
  return ret;
}

bool QSftpFileEngine::remove() {
  std::string p(path.toStdString());
  QMutexLocker locker(host->mutex());
  if(sftp_unlink(host->sftpSession(), p.c_str()) < 0) {
    setError(QFile::ResourceError, host->getSftpError());
    return false;
  }
  return true;
}

bool QSftpFileEngine::rename(const QString& newName) {
  std::string p(path.toStdString());
  std::string newp(newName.toStdString());
  QMutexLocker locker(host->mutex());
  if(sftp_rename(host->sftpSession(), p.c_str(), newp.c_str()) < 0) {
    setError(QFile::ResourceError, host->getSftpError());
    return false;
  }
  return true;
}

bool QSftpFileEngine::isRelativePath() const {
  return !path.isEmpty() && path[0] == '/';
}

bool QSftpFileEngine::seek(qint64 pos) {
  QMutexLocker locker(host->mutex());
  int ret = sftp_seek64(file, pos);
  if(ret < 0)
    setError(QFile::ResourceError, host->getSftpError());
  return ret == 0;
}

qint64 QSftpFileEngine::size() const {
  return getAttrs()->size;
}

qint64 QSftpFileEngine::write(const char* data, qint64 len) {
  QMutexLocker locker(host->mutex());
  ssize_t ret =  sftp_write(file, data, len);
  if(ret < 0)
    setError(QFile::ResourceError, host->getSftpError());
  return ret;
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

QAbstractFileEngine::FileFlags QSftpFileEngine::fileFlags(sftp_attributes_struct* attrs, const QString& baseName, QAbstractFileEngine::FileFlags hints) {
  QAbstractFileEngine::FileFlags flags;
  if(attrs == NULL) return flags;
  flags |= QAbstractFileEngine::ExistsFlag;
  flags |= QAbstractFileEngine::FileFlags((attrs->permissions & 000700) << 6);
  flags |= QAbstractFileEngine::FileFlags((attrs->permissions & 000700) << 2);
  //if(attrs->owner == currentowner)
  //  flags |= QAbstractFileEngine::FileFlags(attrs->permissions & 000700);
  flags |= QAbstractFileEngine::FileFlags((attrs->permissions & 000070) << 1);
  flags |= QAbstractFileEngine::FileFlags(attrs->permissions & 000007);
  switch(attrs->type) {
  case SSH_FILEXFER_TYPE_SYMLINK:   flags |= QAbstractFileEngine::LinkType;      break;
  case SSH_FILEXFER_TYPE_DIRECTORY: flags |= QAbstractFileEngine::DirectoryType; break;
  case SSH_FILEXFER_TYPE_REGULAR:   flags |= QAbstractFileEngine::FileType;      break;
  default: break;
  }
  if(baseName == "/") flags |= QAbstractFileEngine::RootFlag;
  if(baseName.startsWith('.')) flags |= QAbstractFileEngine::HiddenFlag;
  qDebug() << "File exists, returning flags: " << flags;
  return flags;
}

sftp_attributes_struct* QSftpFileEngine::getAttrs(bool refresh) const {
  QMutexLocker locker(host->mutex());
  if(!attrs || refresh){
    if(attrs) sftp_attributes_free(attrs);
    std::string p(path.toStdString());
    attrs = sftp_lstat(host->sftpSession(), p.c_str());
  }
  return attrs;
}

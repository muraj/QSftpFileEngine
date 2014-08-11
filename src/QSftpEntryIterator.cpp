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

#include "QSftpEntryIterator.h"
#include "QSftpSession.h"
#include <QDebug>
#include <string>
#include <libssh/sftp.h>
#include <QMutexLocker>

QSftpEntryIterator::QSftpEntryIterator(QSharedPointer<QSftpSession> ssh, const QString& path, QDir::Filters filters, const QStringList& nameFilters) 
  : QAbstractFileEngineIterator(filters, nameFilters), session(ssh), attrs(NULL) {
  std::string path_str(path.toStdString());
  QMutexLocker locker(session->mutex());
  dir = sftp_opendir(session->sftpSession(), path_str.c_str());
}

QSftpEntryIterator::~QSftpEntryIterator() {
  if(attrs) {
    sftp_attributes_free(attrs);
    attrs = NULL;
  }
  if(dir) {
    sftp_closedir(dir);
    dir = NULL;
  }
}

bool QSftpEntryIterator::hasNext() const {
  return dir && sftp_dir_eof(dir) == 0;
}

QString QSftpEntryIterator::next() {
  if(!hasNext()) return QString();
  if(attrs) {
    sftp_attributes_free(attrs);
    attrs = NULL;
  }
  {
    QMutexLocker locker(session->mutex());
    attrs = sftp_readdir(session->sftpSession(), dir);
  }
  qDebug() << dir->name << '/' << currentFileName();
  return currentFileName();
}

QString QSftpEntryIterator::currentFileName() const {
  return (attrs ? QString(attrs->name) : QString());
}

#if 0
// TODO: psuedo-code to generate the QFileInfo from the attribute struct
QFileInfo QSftpEntryIterator::currentFileInfo() const {
  QAbstractFileEngine::FileFlags flags = QSftpFileEngine::fileFlags(attrs);
  return QFileInfo(flags);
}
#endif

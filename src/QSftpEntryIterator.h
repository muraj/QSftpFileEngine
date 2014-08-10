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

#ifndef QSFTPENTRYITERATOR_H
#define QSFTPENTRYITERATOR_H

#include "qsftp_export.h"
#include <QAbstractFileEngineIterator>
#include <QSharedPointer>

class QSftpSession;
class sftp_dir_struct;
class sftp_attributes_struct;

class QSFTP_EXPORT QSftpEntryIterator : public QAbstractFileEngineIterator {
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

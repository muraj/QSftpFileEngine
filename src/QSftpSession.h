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

#ifndef QSFTPSESSION_H
#define QSFTPSESSION_H

#include "qsftp_export.h"
#include <cstdlib>

struct sftp_session_struct;
struct ssh_session_struct;
class QString;
class QMutex;

class QSFTP_NO_EXPORT QSftpSession {
public:
  QSftpSession(const QString& user, const QString& host);
  ~QSftpSession();
  bool connect();
  bool auth(bool (*prompt_cb)(const char*, char*, size_t, bool));
  bool startSftp();
  int errorCode() const;
  QString getSshError() const;
  QString getSftpError() const;
  QMutex* mutex() const { return _mutex; }
  sftp_session_struct* sftpSession() { return sftp; }
  ssh_session_struct* sshSession() { return ssh; }
private:
  bool authed;
  QMutex* _mutex;
  sftp_session_struct* sftp;
  ssh_session_struct* ssh;
};
#endif // QSFTPSESSION_H


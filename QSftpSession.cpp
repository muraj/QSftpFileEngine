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

#include "QSftpSession.h"
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <string>
#include <iostream>
#include <QString>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>

QSftpSession::QSftpSession(const QString& user, const QString& host) : authed(false), sftp(NULL), _mutex(new QMutex()) {
  ssh = ssh_new();

  std::string hostname = host.toStdString();
  std::string username = user.toStdString();
  qDebug() << "Setting up connection to " << user << '@' << host;
  ssh_options_set(ssh, SSH_OPTIONS_USER, username.c_str());
  ssh_options_set(ssh, SSH_OPTIONS_HOST, hostname.c_str());
}

QSftpSession::~QSftpSession() {
  if(sftp) {
    sftp_free(sftp);
    sftp = NULL;
  }
  if(ssh) {
    if(ssh_is_connected(ssh))
      ssh_disconnect(ssh);
    ssh_free(ssh);
    ssh = NULL;
  }
  delete _mutex;
  _mutex = NULL;
}

bool QSftpSession::connect() {
  QMutexLocker locker(_mutex);
  if(ssh_is_connected(ssh)) return true;
  qDebug() << "Connecting to host...";
  return ssh_connect(ssh) == SSH_OK;
}

static int authPassword(ssh_session session, bool (*prompt_cb)(const char*, char*, size_t, bool)) {
  qDebug() << "Trying password authentication...";
  static char buffer[1024] = { 0 };
  if(!prompt_cb("Password: ", buffer, sizeof(buffer), true)) return false;
  int ret = ssh_userauth_password(session, NULL, buffer);
  memset(buffer, 0, sizeof(buffer));  // Security purposes
  return ret;
}

static int authInteractive(ssh_session session, bool (*prompt_cb)(const char*, char*, size_t, bool)) {
  qDebug() << "Trying interactive authentication...";
  static char buffer[1024] = { 0 };
  int rc = ssh_userauth_kbdint(session, NULL, NULL);
  while(rc == SSH_AUTH_INFO) {
    const char* name  = ssh_userauth_kbdint_getname(session);
    const char* instr = ssh_userauth_kbdint_getinstruction(session);
    int n = ssh_userauth_kbdint_getnprompts(session);
    for(int i=0;i<n;i++) {
      char echo;
      const char* prompt = ssh_userauth_kbdint_getprompt(session, i, &echo);
      if(!prompt_cb(prompt, buffer, sizeof(buffer), echo != 0)) return false;
      rc = ssh_userauth_kbdint_setanswer(session, i, buffer);
      memset(buffer, 0, sizeof(buffer));  // Security purposes
      if(rc < 0) return false;
    }
    rc = ssh_userauth_kbdint(session, NULL, NULL);
  }
  return rc;
}

bool QSftpSession::auth(bool (*prompt_cb)(const char*, char*, size_t, bool)) {
  QMutexLocker locker(_mutex);
  if(authed) return true;
  qDebug() << "Trying host-based auth...";
  if(ssh_userauth_none(ssh, NULL) == SSH_AUTH_SUCCESS) return true;
  qDebug() << "Getting auth list...";
  int method = ssh_userauth_list(ssh, NULL);
  qDebug() << "Iterating auth list, starting with host-based...";
  if((method & SSH_AUTH_METHOD_HOSTBASED) &&
      ssh_userauth_none(ssh, NULL) == SSH_AUTH_SUCCESS);
  else if((method & SSH_AUTH_METHOD_PUBLICKEY) &&
      ssh_userauth_publickey_auto(ssh, NULL, NULL) == SSH_AUTH_SUCCESS);
  else if((method & SSH_AUTH_METHOD_PASSWORD) &&
      authPassword(ssh, prompt_cb) == SSH_AUTH_SUCCESS);
  else if((method & SSH_AUTH_METHOD_INTERACTIVE) &&
      authInteractive(ssh, prompt_cb) == SSH_AUTH_SUCCESS);
  else return false;
  authed = true;
  return true;
}

bool QSftpSession::startSftp() {
  QMutexLocker locker(_mutex);
  if(sftp) return true;
  qDebug() << "Building an sftp session";
  sftp = sftp_new(ssh);
  return sftp_init(sftp) != SSH_OK;
}

int QSftpSession::errorCode() const {
  return ssh_get_error_code(ssh);
}

QString QSftpSession::getSshError() const {
  return QString(ssh_get_error(ssh));
}
QString QSftpSession::getSftpError() const {
  return QString(sftp_get_error(sftp));
}

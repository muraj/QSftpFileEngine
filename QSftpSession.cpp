#include "QSftpSession.h"
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <string>
#include <iostream>

QSftpSession::QSftpSession(const QString& host) : sftp(NULL) {
  ssh = ssh_new();
  std::string hostname = host.section('@', 1).toStdString();

  if(hostname.empty())
    hostname = host.toStdString();
  else
    parsed_username = host.section('@', 0, 0);

  ssh_options_set(ssh, SSH_OPTIONS_HOST, hostname.c_str());
}

QSftpSession::~QSftpSession() {
  if(ssh && ssh_is_connected(ssh))
    ssh_disconnect(ssh);
  ssh_free(ssh);
}

bool QSftpSession::connect(QString& err) {
  QString user = parsed_username;
  if(!ssh_is_connected(ssh)) {
    if(user.isEmpty())
      user = askUser(tr("Username: "));
    std::string user_str(user.toStdString());
    ssh_options_set(ssh, SSH_OPTIONS_USER, user_str.c_str());
    if(ssh_connect(ssh) != SSH_OK) {
      err = tr("Error connecting to remote server: %1")
              .arg(QString(ssh_get_error(ssh)));
      return false;
    }
  }

  if(ssh_userauth_none(ssh, NULL) != SSH_AUTH_SUCCESS) {
    int method = ssh_userauth_list(ssh, NULL);
    if((method & SSH_AUTH_METHOD_PUBLICKEY) &&
        ssh_userauth_publickey_auto(ssh, NULL, NULL) == SSH_OK);
    else if((method & SSH_AUTH_METHOD_INTERACTIVE) &&
        authInteractive());
    else if((method & SSH_AUTH_METHOD_PASSWORD) &&
        authPassword());
    else return false;
  }

  sftp = sftp_new(ssh);
  if(sftp_init(sftp) != SSH_OK)
  {
    err = tr("Error initializing SFTP session: %1")
            .arg(sftp_get_error(sftp));
    return false;
  }
  
  return true;

}

QString QSftpSession::askUser(const QString& prompt) {
  char buffer[1024];
  if(askUser(prompt, buffer, sizeof(buffer), false))
    return QString(buffer);
  return QString();
}

bool QSftpSession::askUser(const QString& prompt, char* answer, int maxLen, bool secure) {
  std::cout << prompt.toStdString();
  return std::cin.getline(answer, maxLen);
}

bool QSftpSession::authInteractive() {
  int rc = ssh_userauth_kbdint(ssh, NULL, NULL);
  char answerBuffer[1024] = {0};
  while(rc == SSH_AUTH_INFO) {
    //const char* name = ssh_userauth_kbdint_getname(ssh);
    //const char* instr = ssh_userauth_kbdint_getinstruction(ssh);
    int nprompts = ssh_userauth_kbdint_getnprompts(ssh);
    for(int i = 0; i < nprompts; i++) {
      char echo;
      const char* prompt = ssh_userauth_kbdint_getprompt(ssh, i, &echo);
      if(!askUser(prompt, answerBuffer, sizeof(answerBuffer), echo != 0))
        return false;
      int ret = ssh_userauth_kbdint_setanswer(ssh, i, answerBuffer);
      // Overwrite data for security purposes
      std::fill(answerBuffer, answerBuffer + sizeof(answerBuffer), 0);
      if(ret < 0) return false;
    }
    rc = ssh_userauth_kbdint(ssh, NULL, NULL);
  }
  return true;
}

bool QSftpSession::authPassword() {
  char answerBuffer[256];
  askUser(tr("Password: "), answerBuffer, sizeof(answerBuffer), true);
  int ret = ssh_userauth_password(ssh, NULL, answerBuffer);
  // Overwrite data for security purposes
  std::fill(answerBuffer, answerBuffer + sizeof(answerBuffer), 0);
  return ret == SSH_OK;
}

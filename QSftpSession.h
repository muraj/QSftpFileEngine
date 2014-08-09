#ifndef QSFTPSESSION_H
#define QSFTPSESSION_H

#include <cstdlib>

struct sftp_session_struct;
struct ssh_session_struct;
class QString;

class QSftpSession {
public:
  QSftpSession(const QString& user, const QString& host);
  ~QSftpSession();
  bool connect();
  bool auth(bool (*prompt_cb)(const char*, char*, size_t, bool));
  bool startSftp();
  int errorCode() const;
  QString getSshError() const;
  QString getSftpError() const;
  sftp_session_struct* sftpSession() { return sftp; }
  ssh_session_struct* sshSession() { return ssh; }
private:
  bool authed;
  sftp_session_struct* sftp;
  ssh_session_struct* ssh;
};
#endif // QSFTPSESSION_H


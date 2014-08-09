#ifndef QSFTPSESSION_H
#define QSFTPSESSION_H

#include <QCoreApplication>
#include <QString>

struct sftp_session_struct;
struct ssh_session_struct;

class QSftpSession {
  Q_DECLARE_TR_FUNCTIONS(QSftpSession);
public:
  QSftpSession(const QString& host);
  ~QSftpSession();
  bool connect(QString& err);
  QString askUser(const QString& prompt);
  virtual bool askUser(const QString& prompt, char* answer, int maxLen, bool secure=true);
  sftp_session_struct* sftpSession() { return sftp; }
  ssh_session_struct* sshSession() { return ssh; }
private:
  QString parsed_username;
  sftp_session_struct* sftp;
  ssh_session_struct* ssh;

  bool authInteractive();
  bool authPassword();
};
#endif // QSFTPSESSION_H


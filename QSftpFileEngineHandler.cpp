#include "QSftpFileEngineHandler.h"
#include "QSftpFileEngine.h"
#include "QSftpSession.h"

QSftpFileEngineHandler::QSftpFileEngineHandler() : QAbstractFileEngineHandler() {

}

QSftpFileEngineHandler::~QSftpFileEngineHandler() {

}

QAbstractFileEngine* QSftpFileEngineHandler::create(const QString& fileName) const {
  // TODO: Parse hostname + path
  QString path = fileName.section(':', 1);
  if(path.isEmpty()) return NULL;   // Doesn't start with a hostname
  QString hostname = fileName.section(':', 0, 0);
  QSharedPointer<QSftpSession> ssh = QSftpFileEngine::session(hostname);
  if(ssh.isNull()) return NULL;     // Couldn't build an ssh session
  // AUTH HERE!
  // return new QSftpFileEngine(ssh, fileName);
  return NULL;
}

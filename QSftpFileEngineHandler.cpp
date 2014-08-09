#include "QSftpFileEngineHandler.h"
#include "QSftpFileEngine.h"
#include "QSftpSession.h"

#include <QThread>
#include <QMutex>

#include <libssh/libssh.h>
#include <libssh/callbacks.h>

#include <iostream>

static bool terminal_prompt(const char* msg, char* answer, size_t maxlen, bool) {
  memset(answer, 0, maxlen);
  std::cout << msg;
  std::cin.getline(answer, maxlen);
  return !std::cin.fail();
}

static int qlock_init(void** lock) {
  *lock = new QMutex;
  return (*lock == 0);
}

static int qlock_destory(void** lock) {
  if(!lock || !*lock) return 1;
  QMutex* mutex = static_cast<QMutex*>(*lock);
  delete mutex;
  *lock = NULL;
  return 0;
}

static int qlock_lock(void** lock) {
  if(!lock || !*lock) return 1;
  QMutex* mutex = static_cast<QMutex*>(*lock);
  mutex->lock();
  return 0;
}

static int qlock_unlock(void** lock) {
  if(!lock || !*lock) return 1;
  QMutex* mutex = static_cast<QMutex*>(*lock);
  mutex->unlock();
  return 0;
}

static unsigned long qlock_threadid() {
  return (unsigned long)(QThread::currentThreadId());
}


QSftpFileEngineHandler::QSftpFileEngineHandler(QSftpPromptCallback cb) 
: prompt(cb ? cb : terminal_prompt) {
  // May have to replace on windows with qt call backs...
  ssh_threads_set_callbacks(ssh_threads_get_pthread());
  ssh_init();
}

QSftpFileEngineHandler::~QSftpFileEngineHandler() {

}

QAbstractFileEngine* QSftpFileEngineHandler::create(const QString& fileName) const {
  QString user, host, path;
  QSftpFileEngine::parseFileName(fileName, user, host, path);
  if(host.isEmpty()) return NULL;
  if(user.isEmpty()) return NULL;
#if 0
  if(user.isEmpty()) {
    static char buffer[1024] = { 0 };
    if(!prompt("Username: ", buffer, sizeof(buffer), false))
      return NULL;
    user = QString(buffer);
  }
#endif
  QSharedPointer<QSftpSession> ssh = QSftpFileEngine::session(user, host);
  if(ssh.isNull()) return NULL;     // Couldn't build an ssh session
  if(ssh->connect())
    ssh->auth(prompt);
  return new QSftpFileEngine(ssh, path);
}

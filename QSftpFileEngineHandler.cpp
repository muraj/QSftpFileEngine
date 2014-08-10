#include "QSftpFileEngineHandler.h"
#include "QSftpFileEngine.h"
#include "QSftpSession.h"

#include <QThread>
#include <QMutex>
#include <QDebug>

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

static int qlock_destroy(void** lock) {
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

static const char qthread_cbs_type[] = "qt";
static ssh_threads_callbacks_struct qthread_cbs;

QSftpFileEngineHandler::QSftpFileEngineHandler(QSftpPromptCallback cb) 
: prompt(cb ? cb : terminal_prompt) {
#if 0   // Doesn't seem to want to work
  // Setup libssh's threading library so we don't need to manage it
  // Note: May need to use qt thread handling on windows...
#if 1
  qthread_cbs.type = qthread_cbs_type;
  qthread_cbs.mutex_init = &qlock_init;
  qthread_cbs.mutex_destroy = &qlock_destroy;
  qthread_cbs.mutex_lock = &qlock_lock;
  qthread_cbs.mutex_unlock = &qlock_unlock;
  qthread_cbs.thread_id = &qlock_threadid;
  ssh_threads_set_callbacks(&qthread_cbs);
#else
  ssh_threads_set_callbacks(ssh_threads_get_pthread());
#endif
#endif
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
  return new QSftpFileEngine(ssh, user + '@' + host, path);
}

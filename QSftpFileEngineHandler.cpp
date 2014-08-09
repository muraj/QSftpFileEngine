#include "QSftpFileEngineHandler.h"

QSftpFileEngineHandler::QSftpFileEngineHandler() : QAbstractFileEngineHandler() {

}

QSftpFileEngineHandler::~QSftpFileEngineHandler() {

}

QAbstractFileEngine* QSftpFileEngineHandler::create(const QString& fileName) const {
  // TODO: Parse hostname + path
  return NULL;
}

#ifndef QSFTPFILEENGINEHANDLER_H
#define QSFTPFILEENGINEHANDLER_H

#include <QAbstractFileEngineHandler>

class QSftpFileEngineHandler : public QAbstractFileEngineHandler {
public:
  QSftpFileEngineHandler();
  virtual ~QSftpFileEngineHandler();
  QAbstractFileEngine* create(const QString& fileName) const;
};

#endif // QSFTPFILEENGINEHANDLER_H

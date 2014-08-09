#ifndef QSFTPFILEENGINEHANDLER_H
#define QSFTPFILEENGINEHANDLER_H

#include <QAbstractFileEngineHandler>

typedef bool (*QSftpPromptCallback)(const char*, char*, size_t, bool);

class QSftpFileEngineHandler : public QAbstractFileEngineHandler {
public:
  QSftpFileEngineHandler(QSftpPromptCallback cb = NULL);
  virtual ~QSftpFileEngineHandler();
  QAbstractFileEngine* create(const QString& fileName) const;
private:
  QSftpPromptCallback prompt;
};

#endif // QSFTPFILEENGINEHANDLER_H

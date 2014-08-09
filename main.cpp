#include <QApplication>
#include "filebrowser.h"
#include "QSftpFileEngineHandler.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  //QSftpFileEngineHandler sftpHandler("zijin.it.cx:/");
  QSftpFileEngineHandler sftpHandler;
  FileBrowser browser;
  browser.show();
  return app.exec();
}

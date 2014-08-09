#include <QApplication>
#include "filebrowser.h"
#include "QSftpFileEngineHandler.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QSftpFileEngineHandler sftpHandler;
#if 0
  FileBrowser browser("muraj@zijin.it.cx:/home/muraj");
#else
  FileBrowser browser;
#endif
  browser.show();
  return app.exec();
}

// vim: set filetype=cpp:expandtab:tabstop=2
//  Copyright 2014 Cory Perry
//  This file is part of QSftpFileEngine.
//
//  QSftpFileEngine is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  QSftpFileEngine is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with QSftpFileEngine.  If not, see <http://www.gnu.org/licenses/>.

#include <QApplication>
#include "filebrowser.h"
#include "QSftpFileEngineHandler.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QSftpFileEngineHandler sftpHandler;
#if 1
  FileBrowser browser("muraj@zijin.it.cx:/home/muraj");
#else
  FileBrowser browser;
#endif
  browser.show();
  return app.exec();
}

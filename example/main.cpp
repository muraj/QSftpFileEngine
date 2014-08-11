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
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <iostream>
#include "filebrowser.h"
#include "QSftpFileEngineHandler.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QSftpFileEngineHandler sftpHandler;
#if 0
  FileBrowser browser("muraj@zijin.it.cx:/home/muraj");
  browser.show();
  return app.exec();
#else
  #if 1
    QString fileName = QFileDialog::getOpenFileName(0, QObject::tr("Open File"),
        "muraj@zijin.it.cx:/home/muraj", QString(), 0,
        QFileDialog::DontUseNativeDialog);
  #else
    QString fileName("muraj@zijin.it.cx:/home/muraj/watson.txt");
  #endif
  if(fileName.isEmpty()) return -1;
  QFile file(fileName);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qFatal("Failed to open %s", fileName.toLatin1().data());
    return -1;
  }
  std::cout << "Reading from file: " << fileName.toStdString() << std::endl;
  QTextStream in(&file);
  while(!in.atEnd())
    std::cout << in.readLine().toStdString() << std::endl;
  return 0;
#endif
}

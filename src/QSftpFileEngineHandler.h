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

#ifndef QSFTPFILEENGINEHANDLER_H
#define QSFTPFILEENGINEHANDLER_H

#include <QAbstractFileEngineHandler>
#include "qsftp_export.h"

typedef bool (*QSftpPromptCallback)(const char*, char*, size_t, bool);

class QSFTP_EXPORT QSftpFileEngineHandler : public QAbstractFileEngineHandler {
public:
  QSftpFileEngineHandler(QSftpPromptCallback cb = NULL);
  virtual ~QSftpFileEngineHandler();
  QAbstractFileEngine* create(const QString& fileName) const;
private:
  QSftpPromptCallback prompt;
};

#endif // QSFTPFILEENGINEHANDLER_H

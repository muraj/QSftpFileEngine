# vim: set filetype=cmake:expandtab:tabstop=2
#  Copyright 2014 Cory Perry
#  This file is part of QSftpFileEngine.
#
#  QSftpFileEngine is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  QSftpFileEngine is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with QSftpFileEngine.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8.12)

project(qsftp CXX)

include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(cstdlib HAVE_CSTDLIB)
if(NOT HAVE_CSTDLIB)
  message(FATAL_ERROR "<cstdlib> not found")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/Modules)

find_package(Qt4 REQUIRED QtCore QtGui)
include(${QT_USE_FILE})
include_directories(${PROJECT_BINARY_DIR})

find_package(LIBSSH REQUIRED OPTIONAL_COMPONENTS threads)
include_directories(${LIBSSH_INCLUDE_DIRS})

set(qsftp_UI
  filebrowser.ui
  logindialog.ui
)

set(qsftp_MOC_HDRS
  filebrowser.h
  logindialog.h
)

set(qsftp_HDRS
  QSftpFileEngineHandler.h
  QSftpFileEngine.h
  QSftpEntryIterator.h
  QSftpSession.h
)

set(qsftp_SRCS
  main.cpp
  filebrowser.cpp
  logindialog.cpp
  QSftpFileEngineHandler.cpp
  QSftpFileEngine.cpp
  QSftpEntryIterator.cpp
  QSftpSession.cpp
)

QT4_WRAP_CPP(qsftp_MOC_SRCS ${qsftp_MOC_HDRS})
QT4_WRAP_UI(qsftp_UI_SRCS ${qsftp_UI})

add_executable(qsftp ${qsftp_SRCS} ${qsftp_UI_SRCS} ${qsftp_MOC_SRCS})
target_link_libraries(qsftp ${QT_LIBRARIES} ${LIBSSH_LIBRARIES})

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

project(qsftpfs CXX)

include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(cstdlib HAVE_CSTDLIB)
if(NOT HAVE_CSTDLIB)
  message(FATAL_ERROR "<cstdlib> not found")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/Modules)

find_package(Qt4 REQUIRED QtCore QtGui)
include(${QT_USE_FILE})

find_package(LIBSSH REQUIRED)
include_directories(${LIBSSH_INCLUDE_DIRS})
add_definitions(${LIBSSH_DEFINITIONS})

add_subdirectory(src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

option(BUILD_EXAMPLES "Enable building of examples" ON)
if(BUILD_EXAMPLES)
  add_subdirectory(example)
endif()

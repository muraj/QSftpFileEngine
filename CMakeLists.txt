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

find_package(Git)
if(GIT_FOUND)
  execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --dirty=M
                  OUTPUT_VARIABLE qsftpfs_TAG_STRING OUTPUT_QUIET ERROR_QUIET)
  string(REGEX REPLACE "^v([0-9]+).[0-9]+.[0-9]+.*" "\\1" qsftpfs_VERSION_MAJOR "${qsftpfs_TAG_STRING}")
  string(REGEX REPLACE "^v[0-9]+.([0-9]+).[0-9]+.*" "\\1" qsftpfs_VERSION_MINOR "${qsftpfs_TAG_STRING}")
  string(REGEX REPLACE "^v[0-9]+.[0-9]+.([0-9]+).*" "\\1" qsftpfs_VERSION_PATCH "${qsftpfs_TAG_STRING}")
  execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short --verify HEAD
                  OUTPUT_VARIABLE qsftpfs_VERSION_SHA1)
endif()

include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(cstdlib HAVE_CSTDLIB)
if(NOT HAVE_CSTDLIB)
  message(FATAL_ERROR "<cstdlib> not found")
endif()

option(BUILD_EXAMPLES "Enable building of examples" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/Modules)

set(QT_DONT_USE_QTCORE FALSE)
if(NOT BUILD_EXAMPLES)
  set(QT_DONT_USE_QTGUI TRUE)
else()
  set(QT_DONT_USE_QTGUI FALSE)
endif()

find_package(Qt4 4.7 REQUIRED)
include(${QT_USE_FILE})

find_package(LIBSSH REQUIRED)
include_directories(${LIBSSH_INCLUDE_DIRS})
add_definitions(${LIBSSH_DEFINITIONS})

add_subdirectory(src)

if(BUILD_EXAMPLES)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)
  add_subdirectory(example)
endif()

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


find_package(PkgConfig)
pkg_check_modules(PC_LIBSSH QUIET libssh)
set(LIBSSH_DEFINITIONS ${PC_LIBSSH_CFLAGS_OTHER})

find_path(LIBSSH_INCLUDE_DIR
  NAMES libssh.h
  HINTS ${PC_LIBSSH_INCLUDE_DIR} ${PC_LIBSSH_INCLUDE_DIRS}
  PATH_SUFFIXES libssh)

find_library(LIBSSH_LIBRARY
  HINTS ${PC_LIBSSH_LIBDIR} ${PC_LIBSSH_LIBRARY_DIRS}
  NAMES ssh)

list(FIND LIBSSH_FIND_COMPONENTS "threads" ThreadsRequested)

if(NOT ThreadsRequested EQUAL -1)
  find_library(LIBSSH_THREADS_LIBRARY
    HINTS ${PC_LIBSSH_LIBDIR} ${PC_LIBSSH_LIBRARY_DIRS}
    NAMES ssh_threads)
  if(LIBSSH_FIND_REQUIRED_threads AND NOT LIBSSH_THREADS_LIBRARY)
    message(FATAL_ERROR "ssh_threads library not found")
  endif()
endif()

set(LIBSSH_LIBRARIES ${LIBSSH_LIBRARY} ${LIBSSH_THREADS_LIBRARY})
set(LIBSSH_INCLUDE_DIRS ${LIBSSH_INCLUDE_DIR})
set(LIBSSH_VERSION "${PC_LIBSSH_VERSION}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBSSH
  REQUIRED_VARS LIBSSH_LIBRARIES LIBSSH_INCLUDE_DIRS
  VERSION_VAR LIBSSH_VERSION)

mark_as_advanced(LIBSSH_INCLUDE_DIRS LIBSSH_LIBRARIES)

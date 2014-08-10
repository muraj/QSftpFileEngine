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

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(qsftpfs_HDRS
  QSftpFileEngineHandler.h
  QSftpFileEngine.h
  QSftpEntryIterator.h
  QSftpSession.h
)
set(qsftpfs_SRCS
  QSftpFileEngineHandler.cpp
  QSftpFileEngine.cpp
  QSftpEntryIterator.cpp
  QSftpSession.cpp
)

if(EXCLUDE_DEPRECATED)
  set(NO_BUILD_DEPRECATED DEFINE_NO_DEPRECATED)
endif()

include(GenerateExportHeader)

add_compiler_export_flags()

# To be used with tests, examples, etc
add_library(qsftpfs_obj OBJECT ${qsftpfs_SRCS})

add_library(qsftpfss STATIC $<TARGET_OBJECTS:qsftpfs_obj>)
add_library(qsftpfs SHARED ${qsftpfs_SRCS})

set_target_properties(qsftpfss PROPERTIES COMPILE_FLAGS -DQSFTP_STATIC_DEFINE)

generate_export_header(qsftpfs ${NO_BUILD_DEPRECATED} BASE_NAME QSFTP)

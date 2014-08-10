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

set(qsftpfs_UI
  filebrowser.ui
)
set(qsftpfs_MOC_HDRS
  filebrowser.h
)
set(qsftpfs_SRCS
  main.cpp
  filebrowser.cpp
)

QT4_WRAP_CPP(qsftpfs_MOC_SRCS ${qsftpfs_MOC_HDRS})
QT4_WRAP_UI(qsftpfs_UI_SRCS ${qsftpfs_UI})

add_executable(filebrowser $<TARGET_OBJECTS:qsftpfs_obj> ${qsftpfs_SRCS} ${qsftpfs_MOC_SRCS} ${qsftpfs_UI_SRCS})

target_link_libraries(filebrowser
    ${QT_LIBRARIES} ${LIBSSH_LIBRARIES})

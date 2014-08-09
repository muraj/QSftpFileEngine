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

find_package(LIBSSH REQUIRED)
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

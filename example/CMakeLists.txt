
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(qsftpfs_UI
  filebrowser.ui
  logindialog.ui
)
set(qsftpfs_MOC_HDRS
  filebrowser.h
  logindialog.h
)
set(qsftpfs_SRCS
  main.cpp
  filebrowser.cpp
  logindialog.cpp
)

QT4_WRAP_CPP(qsftpfs_MOC_SRCS ${qsftpfs_MOC_HDRS})
QT4_WRAP_UI(qsftpfs_UI_SRCS ${qsftpfs_UI})

add_executable(filebrowser $<TARGET_OBJECTS:qsftpfs_obj> ${qsftpfs_SRCS} ${qsftpfs_MOC_SRCS} ${qsftpfs_UI_SRCS})

target_link_libraries(filebrowser
    ${QT_LIBRARIES} ${LIBSSH_LIBRARIES})

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

add_library(qsftpfs_obj OBJECT ${qsftpfs_SRCS})

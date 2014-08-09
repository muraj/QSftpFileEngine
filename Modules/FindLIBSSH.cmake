find_package(PkgConfig)
pkg_check_modules(PC_LIBSSH QUIET libssh)
set(LIBSSH_DEFINITIONS ${PC_LIBSSH_CFLAGS_OTHER})

find_path(LIBSSH_INCLUDE_DIR
  NAMES libssh.h
  HINTS ${PC_LIBSSH_INCLUDE_DIR} ${PC_LIBSSH_INCLUDE_DIRS}
  PATH_SUFFIXES libssh)

find_library(LIBSSH_LIBRARY
  HINTS ${PC_LIBSSH_LIBDIR} ${PC_LIBSSH_LIBRARY_DIRS}
  NAMES ssh ssh_threads)

set(LIBSSH_LIBRARIES ${LIBSSH_LIBRARY})
set(LIBSSH_INCLUDE_DIRS ${LIBSSH_INCLUDE_DIR})
set(LIBSSH_VERSION "${PC_LIBSSH_VERSION}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBSSH FOUND_VAR LIBSSH_FOUND
  REQUIRED_VARS LIBSSH_LIBRARIES LIBSSH_INCLUDE_DIRS
  VERSION_VAR LIBSSH_VERSION)

mark_as_advanced(LIBSSH_INCLUDE_DIRS LIBSSH_LIBRARIES)

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (calib_project)

# set dir to store library
SET (LIBRARY_OUTPUT_PATH ../lib)

# set dir for header
INCLUDE_DIRECTORIES (../include)

# sources
SET (LIBSRCS sys_info.c build_info.c)

# build libcalib.a with LIBSRCS sources
ADD_LIBRARY (calib STATIC ${LIBSRCS})




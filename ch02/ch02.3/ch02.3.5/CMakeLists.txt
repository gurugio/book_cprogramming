CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (calib_project)

#
# global setting
#

# show build option
SET(CMAKE_VERBOSE_MAKEFILE ON)

# setup build mode
if ("${build}" MATCHES "debug")
 SET (CMAKE_BUILD_TYPE "debug")
else ("${build}" MATCHES "debug")
 SET (CMAKE_BUILD_TYPE "release")
endif ("${build}" MATCHES "debug")

ADD_DEFINITIONS(-DCALIB_CFG_BUILD_MODE="${CMAKE_BUILD_TYPE}")

# setup compile bit
if ("${bit}" MATCHES "32")
 ADD_DEFINITIONS(-DCALIB_CFG_COMPILE_BIT=32 -m32)
 SET (CMAKE_EXE_LINKER_FLAGS -m32)
else ("${bit}" MATCHES "32")
 ADD_DEFINITIONS(-DCALIB_CFG_COMPILE_BIT=64 -m64)
 SET (CMAKE_EXE_LINKER_FLAGS -m64)
endif ("${bit}" MATCHES "32")

# setup platform info
ADD_DEFINITIONS(-Wall -DCALIB_CFG_OS="${CMAKE_SYSTEM_NAME}" -DCALIB_CFG_CPU="${CMAKE_SYSTEM_PROCESSOR}")

#
# run sub-build
#

# build library
ADD_SUBDIRECTORY(src)

# build unit-test
ADD_SUBDIRECTORY(test)


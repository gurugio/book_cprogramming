CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (calib_project)

ENABLE_TESTING ()

INCLUDE_DIRECTORIES (../include)

ADD_EXECUTABLE (test_build_info test_build_info.c)
TARGET_LINK_LIBRARIES (test_build_info calib)

ADD_EXECUTABLE (test_sys_info test_sys_info.c)
TARGET_LINK_LIBRARIES (test_sys_info calib)

ADD_TEST (unittest1 test_build_info)
ADD_TEST (unittest2 test_sys_info)

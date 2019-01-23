#!/usr/bin/bash
../../../../join_text/join_text.scm book.txt Makefile calib.mk.in config.mk.in configure.in template.mk src/Makefile src/build_info.c src/sys_info.c include/calib.h include/calib_common.h include/platform.h.in include/sys_info.h lib/Makefile test/Makefile test/test_build_info.c test/test_sys_info.c > final_book.txt

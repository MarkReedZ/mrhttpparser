#
# Copyright (c) 2013-2018 Mark Reed
#
# The software is licensed under the MIT License
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

CC?=gcc
PROVE?=prove

CPPFLAGS ?=
LDFLAGS ?=

CPPFLAGS += -I.
CPPFLAGS_DEBUG = $(CPPFLAGS) 
CPPFLAGS_DEBUG += $(CPPFLAGS_DEBUG_EXTRA)
CPPFLAGS_FAST = $(CPPFLAGS) 
CPPFLAGS_FAST += $(CPPFLAGS_FAST_EXTRA)
CPPFLAGS_BENCH = $(CPPFLAGS_FAST)

CFLAGS += -Wall -Wextra -mavx2 -Wno-sign-compare -mbmi2
CFLAGS_DEBUG = $(CFLAGS) -O0 -g $(CFLAGS_DEBUG_EXTRA)
CFLAGS_FAST = $(CFLAGS) -O3 $(CFLAGS_FAST_EXTRA)
CFLAGS_BENCH = $(CFLAGS_FAST) -Wno-unused-parameter
CFLAGS_LIB = $(CFLAGS_FAST) -fPIC

all:

test: test-bin
	$(PROVE) -v ./test-bin

test-bin: mrhttpparser.c picotest/picotest.c test.c
	$(CC) -Wall $(CFLAGS) $(LDFLAGS) -o $@ $^

bench: mrhttpparser.o bench.o
	$(CC) $(CFLAGS_BENCH) $(LDFLAGS) mrhttpparser.o bench.o -o $@

bench.o: bench.c mrhttpparser.h Makefile
	$(CC) $(CPPFLAGS_BENCH) $(CFLAGS_BENCH) -c bench.c -o $@

mrh: mrhttpparser.c
	$(CC) $(CFLAGS_FAST) $(LDFLAGS) -v mrhttpparser.c -o $@ 

clean:
	rm -f test-bin

.PHONY: test

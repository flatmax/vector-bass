# Copyright (c) 2017 The WASM Authors. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#    * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#    * Neither the name of mad chops coder AU nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

lib_LTLIBRARIES = libVS.la

libVS_la_SOURCES = Location.C vectorSynth.C XFigOOutput.C
libVS_la_CPPFLAGS = -I$(top_srcdir)/include
libVS_la_LDFLAGS =  -version-info $(LT_CURRENT) -release $(LT_RELEASE)

if HAVE_EMSCRIPTEN
all-local: libVS.la

	@emcc --bind -s ALLOW_MEMORY_GROWTH=1 --llvm-opts 1 -s MODULARIZE=1 -s EXPORT_NAME="'libVS'" -s WASM=0 --memory-init-file 0 $(AM_CXXFLAGS) .libs/libVS.so -o .libs/libVS.js
	mv .libs/libVS.js .libs/libVSNode.js
	cp .libs/libVSNode.js ../webcomponent/
	@emcc --bind --llvm-opts 1 --memory-init-file 0 -s EXPORT_ES6=1 -s MODULARIZE=1 -s EXPORT_NAME="'libVS'" -s SINGLE_FILE=1 -s "BINARYEN_METHOD='native-wasm'" -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 $(AM_CXXFLAGS) .libs/libVS.so -o .libs/libVS.js
	cp .libs/libVS.js ../webcomponent/
install-exec-hook:
	cp .libs/libVS.js $(DESTDIR)$(libdir)
clean-local:
	@rm -f .libs/libVS.node
endif

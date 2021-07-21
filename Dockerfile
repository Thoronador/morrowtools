# Note: This Dockerfile is still incomplete, but it attempts to build the
#       project by cross-compiling it for 32-bit Windows OS.

# build stage
FROM debian:10-slim AS builder
LABEL maintainer="Dirk Stolle <striezel-dev@web.de>"
RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y binutils-mingw-w64-i686 cmake g++-mingw-w64-i686 gcc-mingw-w64-i686 mingw-w64-i686-dev libz-mingw-w64-dev pkg-config zip
RUN mkdir -p /opt/morrowtools
COPY ./ /opt/morrowtools
WORKDIR /opt/morrowtools
ENV CXX=i686-w64-mingw32-c++-win32
ENV CC=i686-w64-mingw32-gcc-win32
RUN mkdir build && cd build && cmake .. && make -j4 && ctest -V
RUN cd /opt/morrowtools/build && zip artifacts.zip \
    mw/cell_translator/cell_translator.exe \
    mw/data_cleaner/data_cleaner.exe \
    mw/skill_rebalance/skill_rebalance.exe \
    mw/spell_rename/spell_rename.exe \
    sr/conv_cams/conv_cams.exe \
    sr/formID_finder/formID_finder.exe \
    sr/race_resize/race_resize.exe \
    /usr/lib/gcc/i686-w64-mingw32/8.3-win32/libstdc++-6.dll \
    /usr/lib/gcc/i686-w64-mingw32/8.3-win32/libgcc_s_sjlj-1.dll

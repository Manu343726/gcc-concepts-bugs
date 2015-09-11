FROM ubuntu:14.04
MAINTAINER Manu Sánchez (Manu343726)

ENV GCC_BUILDTREE_DIR ${HOME}/gcc-buildtree
ENV GCC_CONCEPTS_INSTALL_DIR ${HOME}/gcc-concepts
ENV GCC_SOURCES_DIR ${HOME}/gcc-sources
ENV GCC_SUFFIX -concepts
ENV THREADS 4

# GCC dependencies

RUN apt-get update
RUN yes | apt-get install git
RUN yes | apt-get install libmpfr-dev libgmp3-dev libmpc-dev flex bison
RUN yes | apt-get install build-essential

# Setup

RUN git clone https://github.com/gcc-mirror/gcc $GCC_SOURCES_DIR
RUN mkdir GCC_BUILDTREE_DIR
RUN mkdir GCC_CONCEPTS_INSTALL_DIR

# Configure

WORKDIR GCC_BUILDTREE_DIR
RUN ${GCC_SOURCES_DIR}/configure --prefix="${GCC_CONCEPTS_INSTALL_DIR}" --disable-multilib --disable-werror --program-suffix="${GCC_SUFFIX}" --enable-languages=c++ --enable-version-specific-runtime-libs
RUN make -j${THREADS}
RUN make install

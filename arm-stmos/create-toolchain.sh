#!/bin/bash
#
# Based on:
# binutils 2.31.1
# gcc 8.2.0
#
# Need:
# build-essential
# bison
# flex
# libgmp3-dev
# libmpc-dev
# libmpfr-dev

# Get sources
wget https://ftp.gnu.org/gnu/binutils/binutils-2.31.1.tar.xz
wget https://ftp.gnu.org/gnu/gcc/gcc-8.2.0/gcc-8.2.0.tar.xz
tar xfv ./binutils-2.31.1.tar.xz
tar xfv ./gcc-8.2.0.tar.xz

# Apply binutils changes
for file in $(find ./binutils-files -name '*.*'); do
	cp -v $file $(echo $file | sed s/binutils-files/binutils-2.31.1/)
done

# Apply gcc changes
for file in $(find ./gcc-files -name '*.*'); do
	cp -v $file $(echo $file | sed s/gcc-files/gcc-8.2.0/)
done

# Prepare
export TARGET=arm-stmos
mkdir -p $HOME/$TARGET
export PATH="$HOME/$TARGET/bin:$PATH"

# Build binutils
cd ./binutils-2.31.1/ld
automake

mkdir ../build
cd ../build
echo "Configure binutils... (../configure)"
bash
echo "Building binutils..."
make -j8
make install

# Build gcc
cd ../../gcc-8.2.0/libstdc++-v3/
autoconf

mkdir ../build
cd ../build
echo "Configure gcc... (../configure)"
bash
echo "Building gcc..."
make -j8
make install


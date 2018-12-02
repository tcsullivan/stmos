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
if [ ! -d ./binutils-2.31.1 ]; then
	wget https://ftp.gnu.org/gnu/binutils/binutils-2.31.1.tar.xz
	tar xf ./binutils-2.31.1.tar.xz
	rm ./binutils-2.31.1.tar.xz

	# Apply binutils changes
	for file in $(find ./binutils-files -name '*.*'); do
		cp -v $file $(echo $file | sed s/binutils-files/binutils-2.31.1/)
	done
fi
if [ ! -d ./gcc-8.2.0 ]; then
	wget https://ftp.gnu.org/gnu/gcc/gcc-8.2.0/gcc-8.2.0.tar.xz
	tar xf ./gcc-8.2.0.tar.xz
	rm ./gcc-8.2.0.tar.xz

	# Apply gcc changes
	for file in $(find ./gcc-files -name '*.*'); do
		cp -v $file $(echo $file | sed s/gcc-files/gcc-8.2.0/)
	done
fi



# Prepare
export TARGET=arm-stmos
export PREFIX=$HOME/$TARGET
mkdir -p $PREFIX
export PATH="$PREFIX/bin:$PATH"

# Build binutils
if ! [ -x "$(command -v $TARGET-as)" ]; then
	echo "Starting binutils..."
	cd ./binutils-2.31.1/ld
	aclocal
	automake
	
	mkdir ../build
	cd ../build
	echo "Configuring binutils..."
	../configure --prefix=$PREFIX --target=$TARGET --with-sysroot --disable-nls
	echo "Building binutils..."
	make -j8
	make install
	
	echo "Binutils installed. Test if it works? (Ctrl+D to continue with gcc)"
	bash
else
	echo "Binutils found, skipping..."
	cd binutils-2.31.1/build
fi

# Build gcc
if ! [ -x "$(command -v $TARGET-gcc)" ]; then
	if [ ! -d "$PREFIX/usr/include" ]; then
		# Control will enter here if $DIRECTORY doesn't exist.
		echo "Installing pdclib includes..."
		mkdir -p $PREFIX/usr/include
		cp -Rv ../../../src/pdclib/include/* $PREFIX/usr/include
		cp -Rv ../../../src/pdclib/platform/stmos/include/* \
			$PREFIX/usr/include
	fi

	cd ../../gcc-8.2.0/libstdc++-v3/
	autoreconf
	autoconf
	
	mkdir ../build
	cd ../build
	echo "Configuring gcc..."
	../configure --prefix=$PREFIX --target=$TARGET --with-sysroot=$PREFIX \
		--disable-nls --enable-languages=c,c++
	echo "Building gcc..."
	make all-gcc all-target-libgcc -j8
	make install-gcc install-target-libgcc -j8
else
	echo "gcc found, skipping..."
fi

echo "All done :) enjoy"


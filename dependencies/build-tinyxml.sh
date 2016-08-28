#!/bin/bash

# i6engine
# Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
#
# This file is part of i6engine; i6engine is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

cd "$(readlink -f "$(dirname "${0}")")"

. ./build-common.sh

# TinyXML
ARCHIVE="tinyxml2.zip"
BUILD_DIR="${BUILD_ROOT}/tinyxml2/jni"

PREFIX="${DEP_DIR_OUT}/tinyxml2"

if [ -d ${PREFIX} ]; then
	exit 0
fi

title "Compile TinyXML"

. ./download-dependency.sh ${ARCHIVE}

status "Extracting TinyXML"

cd "${BUILD_ROOT}"

unzip "${ARCHIVE}" >/dev/null

cd "${BUILD_DIR}"

status "Building TinyXML"
gcc -c -fPIC tinyxml2.cpp -o tinyxml2.o
gcc -shared -Wl,-soname,libtinyxml2.so -o libtinyxml2.so tinyxml2.o

status "Installing TinyXML"

mkdir "${PREFIX}"
mkdir "${PREFIX}/include"
mkdir "${PREFIX}/lib"

cp ./*.h "${PREFIX}/include"
cp ./*.so "${PREFIX}/lib"

status "Cleaning up"

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}" >/dev/null

touch "${PREFIX}"

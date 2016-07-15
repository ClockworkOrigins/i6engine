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
BUILD_DIR="${BUILD_ROOT}/tinyxml2"

PREFIX="${DEP_DIR}/tinyxml2"
DEBUG_FLAG=""
RELEASE_FLAG=""
PARALLEL_FLAG=""

if [ -d ${PREFIX} ]; then
	exit 0
fi

if [ ! -z "${BUILD_PARALLEL}" ]; then
	PARALLEL_FLAG="-j ${BUILD_PARALLEL}"
fi

if [ -z "${DEBUG}" ]; then
	BUILD_TYPE="${RELEASE_FLAG}"
else
	BUILD_TYPE="${DEBUG_FLAG}"
fi

if [ ! -z "${CLEAN}" ]; then
	status "Cleaning tinyxml2"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile TinyXML"

./download-dependency.sh ${ARCHIVE}

status "Cleaning TinyXML"
rm -rf "${PREFIX}"

status "Extracting TinyXML"
cd "${BUILD_ROOT}"

# delete old directory
# this should be done during "Cleanup" but doesn't work properly with bamboo
rm -rf "tinyxml2"

unzip "${ARCHIVE}" >/dev/null

cd "${BUILD_DIR}/jni"

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

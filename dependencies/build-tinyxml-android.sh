#!/bin/bash

##
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
##

cd "$(readlink "$(dirname "${0}")")"

. ./build-common.sh android

ARCHIVE="tinyxml2-4.0.1.zip"
BUILD_DIR="${BUILD_ROOT}/tinyxml2-4.0.1"
PREFIX="${DEP_OUT_DIR}/tinyxml2/"

if [ -d ${PREFIX} ]; then
	exit 0
fi

title "Compile tinyxml2"

status "Extracting tinyxml2"

downloadAndUnpack ${ARCHIVE}

status "Configuring clockUtils"

cd "${BUILD_DIR}"
cmake \
	-DBUILD_SHARED_LIBS=OFF \
	-DBUILD_STATIC_LIBS=ON \
	-DCMAKE_INSTALL_PREFIX="${PREFIX}" \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_TOOLCHAIN_FILE=${DEP_OUT_DIR}/../../cmake/android.toolchain.cmake \
	-DANDROID_STL=gnustl_shared \
	.

status "Building tinyxml2"

make -j ${CPU_CORES}

status "Installing tinyxml2"

make install

status "Cleaning up"

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}"


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

ARCHIVE="tinyxml2.zip"
BUILD_DIR="${BUILD_ROOT}/tinyxml2"
PREFIX="${DEP_OUT_DIR}/tinyxml2/"

if [ -d ${PREFIX} ]; then
	exit 0
fi

title "Compile tinyxml2"

. ./download-dependency.sh ${ARCHIVE}

status "Extracting tinyxml2"

cd "${BUILD_ROOT}"
unzip "${ARCHIVE}"

status "Building tinyxml2"

cd "${BUILD_DIR}/jni"
cp -rf "${PATCH_DIR}/tinyxml2/tinyxml2.cpp" "${BUILD_ROOT}/tinyxml2/jni"
${ANDROID_NDK}/ndk-build

mkdir -p "${PREFIX}/include"
mkdir "${PREFIX}/lib"

cp ./jni/*.h "${PREFIX}/include"
cp ./obj/local/armeabi/*.so "${PREFIX}/lib"

status "Cleaning up"

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}"


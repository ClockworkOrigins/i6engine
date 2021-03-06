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

cd "$(readlink "$(dirname "${0}")")"

. ./build-common.sh

# googletest
ARCHIVE="gmock-1.7.0.zip"
BUILD_DIR="${BUILD_ROOT}/gmock-1.7.0"

if [ -d ${BUILD_DIR} ]; then
	rm -rf ${BUILD_DIR}
fi

PREFIX="${DEP_DIR}/gmock/"
PARALLEL_FLAG=""

if [ ! -z "${BUILD_PARALLEL}" ]; then
	PARALLEL_FLAG="-j ${BUILD_PARALLEL}"
fi

DEBUG_FLAG="-DCMAKE_OSX_ARCHITECTURES=i386;x86_64 -DCMAKE_BUILD_TYPE=Debug"
RELEASE_FLAG="-DCMAKE_OSX_ARCHITECTURES=i386;x86_64 -DCMAKE_BUILD_TYPE=Release"

if [ -z "${DEBUG}" ]; then
	BUILD_TYPE="${RELEASE_FLAG}"
else
	BUILD_TYPE="${DEBUG_FLAG}"
fi

if [ ! -z "${CLEAN}" ]; then
	status "Cleaning GoogleMock with GoogleTest"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile GoogleMock with GoogleTest"

./download-dependency.sh ${ARCHIVE}

status "Extracting GoogleMock with GoogleTest"
cd "${BUILD_ROOT}"
unzip "${ARCHIVE}" >/dev/null

status "Configuring GoogleMock with GoogleTest"
cd "${BUILD_DIR}"

cmake \
	-DCMAKE_INSTALL_PREFIX="${PREFIX}" \
	${BUILD_TYPE} \
	. >/dev/null

status "Building GoogleMock with GoogleTest"
make ${PARALLEL_FLAG} >/dev/null

status "Installing GoogleMock with GoogleTest"
mkdir -p ${PREFIX}/lib >/dev/null
cp libgmock.a ${PREFIX}/lib >/dev/null
cp gtest/libgtest.a ${PREFIX}/lib >/dev/null
cp libgmock_main.a ${PREFIX}/lib >/dev/null
cp gtest/libgtest_main.a ${PREFIX}/lib >/dev/null
cp -R include ${PREFIX} >/dev/null
cp -R gtest/include ${PREFIX} >/dev/null

status "Cleaning up"
cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}" >/dev/null

touch "${PREFIX}"

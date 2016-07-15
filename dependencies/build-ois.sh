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

# OIS
ARCHIVE="ois_v1-3.tar.gz"
BUILD_DIR="${BUILD_ROOT}/ois-v1-3"

if [ -d ${BUILD_DIR} ]; then
	rm -rf ${BUILD_DIR}
fi

PREFIX="${DEP_DIR}/ois/"
RELEASE_FLAG=""
DEBUG_FLAG="CFLAGS=-g"
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
	status "Cleaning OIS"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile OIS"

./download-dependency.sh ${ARCHIVE}
./download-dependency.sh ois-gcc-4.7.patch
./download-dependency.sh ois-aclocal-1.13.patch
./download-dependency.sh ois-1.3-linkerror.patch

status "Cleaning OIS"
rm -rf "${PREFIX}"

status "Extracting OIS"
cd "${BUILD_ROOT}"
tar xfz "${ARCHIVE}" >/dev/null

# patch for support with gcc 4.7
patch -p0 -i "ois-gcc-4.7.patch"
patch -p0 -i "ois-aclocal-1.13.patch"
patch -p0 -i "ois-1.3-linkerror.patch"

status "Bootstrapping OIS"
cd "${BUILD_DIR}"
chmod 700 ./*

./bootstrap >/dev/null

status "Configuring OIS"
./configure \
	--prefix="${PREFIX}"\
	${BUILD_TYPE}\
	>/dev/null

status "Building OIS"
make ${PARALLEL_FLAG} >/dev/null

status "Installing OIS"
make ${PARALLEL_FLAG} install >/dev/null

status "Cleaning up"
cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}" >/dev/null

touch "${PREFIX}"


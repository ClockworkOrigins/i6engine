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

# google perftools
ARCHIVE="gperftools-2.4.tar.gz"
BUILD_DIR="${BUILD_ROOT}/gperftools-2.4"

if [ -d ${BUILD_DIR} ]; then
	rm -rf ${BUILD_DIR}
fi

PREFIX="${DEP_DIR}/gperftools/"
PARALLEL_FLAG=""

if [ -d ${PREFIX} ]; then
	exit 0
fi

if [ ! -z "${BUILD_PARALLEL}" ]; then
	PARALLEL_FLAG="-j ${BUILD_PARALLEL}"
fi

RELEASE_FLAG=""
DEBUG_FLAG="CFLAGS=-g"

if [ -z "${DEBUG}" ]; then
	BUILD_TYPE="${RELEASE_FLAG}"
else
	BUILD_TYPE="${DEBUG_FLAG}"
fi

if [ ! -z "${CLEAN}" ]; then
	status "Cleaning GPerfTools"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile GPerfTools"

./download-dependency.sh ${ARCHIVE}

status "Extracting GPerfTools"
cd "${BUILD_ROOT}"
tar xfz "${ARCHIVE}" >/dev/null
cd "${BUILD_DIR}"

# set readable
chmod -R u+w .

status "Configuring GPerfTools"
./configure --libdir="${PREFIX}/lib" --prefix="${PREFIX}" --enable-frame-pointers ${BUILD_TYPE} >/dev/null

status "Building GPerfTools"
make ${PARALLEL_FLAG} >/dev/null

status "Installing GPerfTools"
make ${PARALLEL_FLAG} install >/dev/null

status "Cleaning up"
cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}" >/dev/null

touch "${PREFIX}"


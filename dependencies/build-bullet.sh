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

# bullet
ARCHIVE="bullet-2.82-r2704.tgz"
BUILD_DIR="${BUILD_ROOT}/bullet-2.82-r2704"

if [ -d ${BUILD_DIR} ]; then
	rm -rf ${BUILD_DIR}
fi

PREFIX="${DEP_DIR}/bullet/"
PARALLEL_FLAG=""

if [ ! -z "${BUILD_PARALLEL}" ]; then
	PARALLEL_FLAG="-j ${BUILD_PARALLEL}"
fi

RELEASE_FLAG=Release
DEBUG_FLAG=Debug

if [ -z "${DEBUG}" ]; then
	BUILD_TYPE="${RELEASE_FLAG}"
else
	BUILD_TYPE="${DEBUG_FLAG}"
fi

if [ ! -z "${CLEAN}" ]; then
	status "Cleaning Bullet"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile Bullet"

./download-dependency.sh ${ARCHIVE}

status "Cleaning Bullet"
rm -rf "${PREFIX}" >/dev/null

status "Extracting Bullet"
cd "${BUILD_ROOT}"
tar xfz "${ARCHIVE}" >/dev/null

status "Configuring Bullet"
cd "${BUILD_DIR}"
cmake . \
		-DCMAKE_INSTALL_PREFIX=${PREFIX} \
		-DBUILD_DEMOS=OFF \
		-DINSTALL_LIBS=ON \
		-DBUILD_SHARED_LIBS=ON \
		-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		-DUSE_DOUBLE_PRECISION=ON \
		-DUSE_GRAPHICAL_BENCHMARK=OFF \
		-DUSE_GLUT=OFF \
		-DBUILD_EXTRAS=ON \
		-DINSTALL_EXTRA_LIBS=ON\
		>/dev/null

status "Building Bullet"
make ${PARALLEL_FLAG} >/dev/null

status "Installing Bullet"
make ${PARALLEL_FLAG} install >/dev/null

status "Cleaning up"
cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}" >/dev/null

touch "${PREFIX}"

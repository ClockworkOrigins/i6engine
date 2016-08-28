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

PREFIX="${DEP_DIR_OUT}/gperftools/"

if [ -d ${PREFIX} ]; then
	exit 0
fi

title "Compile GPerfTools"

. ./download-dependency.sh ${ARCHIVE}

status "Extracting GPerfTools"

cd "${BUILD_ROOT}"

tar xfz "${ARCHIVE}" >/dev/null

cd "${BUILD_DIR}"

# set readable
chmod -R u+w .

status "Configuring GPerfTools"
./configure --libdir="${PREFIX}/lib" --prefix="${PREFIX}" --enable-frame-pointers

status "Building GPerfTools"
make -j ${CPU_CORES} >/dev/null

status "Installing GPerfTools"
make -j ${CPU_CORES} install >/dev/null

status "Cleaning up"

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}" >/dev/null

touch "${PREFIX}"


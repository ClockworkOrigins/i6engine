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

ROOT_DIR=${2}
COPY_DIR=${3}

. ./build-common.sh ${1}

ARCHIVE="m2etis-0.4.0-src-rev121.zip"
BUILD_DIR="${BUILD_ROOT}/m2etis-0.4.0-src-rev121"
PREFIX="${DEP_OUT_DIR}/m2etis/"
DEP_DIR="${ROOT_DIR}/../"

if [ -d ${PREFIX} ]; then
	exit 0
fi

title "Compile m2etis"

. ./download-dependency.sh ${ARCHIVE} http://clockwork-origins.de/m2etis/downloads/

status "Extracting m2etis"

cd "${BUILD_ROOT}"
unzip "${ARCHIVE}"

status "Configuring m2etis"

cd "${BUILD_DIR}"
if [ -d "${COPY_DIR}" ]; then
	mkdir -p ${BUILD_DIR}/library/extern
	cp -r ${COPY_DIR}/* ${BUILD_DIR}/library/extern/
fi
if [ -d "${DEP_DIR}/../config/m2etis" ]; then
	cp -rf ${DEP_DIR}/../config/m2etis/* ${BUILD_DIR}/library/generated/
fi
cmake \
	-DWITH_TESTING=OFF \
	-DWITH_SIM=OFF \
	-DWITH_LOGGING=OFF \
	-DWITH_MESSAGECOMPRESSION=OFF \
	-DM2ETIS_BUILD_SHARED=ON \
	-DDEP_DIR="${ROOT_DIR}/" \
	-DADDITIONAL_INCLUDE_DIRECTORIES="${BUILD_DIR}/library/extern" \
	-DCMAKE_INSTALL_PREFIX="${PREFIX}" \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_C_COMPILER=${C_COMPILER} \
	-DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
.

status "Building m2etis"

make -j ${CPU_CORES}

status "Installing m2etis"

make install

status "Cleaning up"

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}"


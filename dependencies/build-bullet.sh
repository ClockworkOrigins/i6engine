#!/bin/bash

##
# Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##

cd "$(readlink -f "$(dirname "${0}")")"

. ./build-common.sh

# bullet
ARCHIVE="bullet-2.82-r2704.tgz"
BUILD_DIR="${BUILD_ROOT}/bullet-2.82-r2704"

if [ -d ${BUILD_DIR} ]; then
	rm -rf ${BUILD_DIR}
fi

PREFIX="${PWD}/bullet/"
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

if ! uptodate "${EX_DIR}/${ARCHIVE}" "${PREFIX}"; then
	status "Bullet seems to be up to date, skipping build"
	exit 0
fi

./download-dependency.sh ${ARCHIVE}

status "Cleaning Bullet"
rm -rf "${DEST_DIR}" >/dev/null

status "Extracting Bullet"
cd "${BUILD_ROOT}"
tar xfz "${EX_DIR}/${ARCHIVE}" >/dev/null

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
rm -r "${BUILD_DIR}" >/dev/null
rm -rf "${DEP_DIR}/../externals"

touch "${PREFIX}"

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

# google perftools
ARCHIVE="gperftools-2.4.tar.gz"
BUILD_DIR="${BUILD_ROOT}/gperftools-2.4"

if [ -d ${BUILD_DIR} ]; then
	rm -rf ${BUILD_DIR}
fi

PREFIX="${DEP_DIR}/gperftools/"
PARALLEL_FLAG=""

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


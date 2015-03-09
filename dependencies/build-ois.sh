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

# OIS
ARCHIVE="ois_v1-3.tar.gz"
BUILD_DIR="${BUILD_ROOT}/ois-v1-3"

if [ -d ${BUILD_DIR} ]; then
	rm -rf ${BUILD_DIR}
fi

PREFIX="${PWD}/ois/"
RELEASE_FLAG=""
DEBUG_FLAG="CFLAGS=-g"
PARALLEL_FLAG=""

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

if ! uptodate "${EX_DIR}/${ARCHIVE}" "${PREFIX}"; then
	status "OIS seems to be up to date, skipping build"
	exit 0
fi

status "Cleaning OIS"
rm -rf "${PREFIX}"

status "Extracting OIS"
cd "${BUILD_ROOT}"
tar xfz "${EX_DIR}/${ARCHIVE}" >/dev/null

# patch for support with gcc 4.7
patch -p0 -i "${PATCH_DIR}/ois-gcc-4.7.patch"
patch -p0 -i "${PATCH_DIR}/ois-aclocal-1.13.patch"

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
rm -r "${BUILD_DIR}" >/dev/null

touch "${PREFIX}"


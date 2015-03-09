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

# boost
ARCHIVE="boost_1_55_0.tar.bz2"
BUILD_DIR="${BUILD_ROOT}/boost_1_55_0"

if [ -d ${BUILD_DIR} ]; then
	rm -rf ${BUILD_DIR}
fi

PREFIX="${PWD}/boost/"
DEBUG_FLAG="variant=debug"
RELEASE_FLAG="variant=release"
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
	status "Cleaning Boost"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile Boost & Boost.Log"

if ! uptodate "${EX_DIR}/${ARCHIVE}" "${PREFIX}" && ! uptodate "${EX_DIR}/${BOOST_LOG_ARCHIVE}" "${PREFIX}"; then
	status "Boost seems to be up to date, skipping build"
	exit 0
fi

status "Cleaning Boost"
rm -rf "${DEST_DIR}" >/dev/null

status "Extracting Boost"
cd "${BUILD_ROOT}"
tar xfj "${EX_DIR}/${ARCHIVE}" >/dev/null

status "Bootstrapping Boost"
cd "${BUILD_DIR}"

./bootstrap.sh --prefix="${PREFIX}" --with-libraries=filesystem,thread,date_time,python,system,serialization,log --with-python=python2.7

status "Building & Installing Boost"
./bjam -d2 \
	${PARALLEL_FLAG} \
	${BUILD_TYPE} \
	link=shared \
	--layout=tagged \
	threading=multi \
	install >/dev/null

if [ 0 -ne $? ]; then
	status "Failed installing boost. Buildfiles not deleted"
	exit 1
fi

status "Cleaning up"
cd "${DEP_DIR}"
rm -rf "${BUILD_DIR}" >/dev/null

touch "${PREFIX}"

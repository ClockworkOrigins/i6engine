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

# TinyXML
ARCHIVE="tinyxml2.zip"
BUILD_DIR="${BUILD_ROOT}/tinyxml2"

PREFIX="${PWD}/tinyxml2"
DEBUG_FLAG=""
RELEASE_FLAG=""
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
	status "Cleaning tinyxml2"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile TinyXML"

if ! uptodate "${EX_DIR}/${ARCHIVE}" "${PREFIX}"; then
	status "TinyXML seems to be up to date, skipping build"
	exit 0
fi

./download-dependency.sh ${ARCHIVE}

status "Cleaning TinyXML"
rm -rf "${DEST_DIR}"
rm -rf "${PREFIX}"

status "Extracting TinyXML"
cd "${BUILD_ROOT}"

# delete old directory
# this should be done during "Cleanup" but doesn't work properly with bamboo
rm -rf "tinyxml2"

unzip "${EX_DIR}/${ARCHIVE}" >/dev/null

cd "${BUILD_DIR}"

status "Building TinyXML"
gcc -c -fPIC tinyxml2.cpp -o tinyxml2.o
gcc -shared -Wl,-soname,libtinyxml2.so -o libtinyxml2.so tinyxml2.o

status "Installing TinyXML"

mkdir "${PREFIX}"
mkdir "${PREFIX}/include"
mkdir "${PREFIX}/lib"

cp ./*.h "${PREFIX}/include"
cp ./*.so "${PREFIX}/lib"

status "Cleaning up"
cd "${DEP_DIR}"
rm -r "${BUILD_DIR}" >/dev/null
rm -rf "${EX_DIR}/.."

touch "${PREFIX}"
